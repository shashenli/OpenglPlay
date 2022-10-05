#include "DecodeDemuxPipeLine.h"
std::mutex DecodeDemuxPipeLine::mtx_for_create_instance_{};
std::weak_ptr<DecodeDemuxPipeLine> DecodeDemuxPipeLine::instance_{};
enum AVPixelFormat DecodeDemuxPipeLine::hw_pix_fmt_ {};
DecodeDemuxPipeLine::DecodeDemuxPipeLinePtr DecodeDemuxPipeLine::CreatInstance()
{
	std::lock_guard<std::mutex> lock_guard(mtx_for_create_instance_);
	if (instance_.expired() == 1)
	{
		std::shared_ptr<DecodeDemuxPipeLine>tmp(new DecodeDemuxPipeLine());
		instance_ = tmp;
		return tmp;
	}
	else
	{
		return instance_.lock();
	}
}

bool DecodeDemuxPipeLine::Init(const char* path)
{
	av_fmt_ = avformat_alloc_context();
	//注册网络流超时回调
	av_fmt_->interrupt_callback.callback = &DecodeDemuxPipeLine::InterruputCallBack;
	av_fmt_->interrupt_callback.opaque = this;
	type_=av_hwdevice_find_type_by_name("d3d11va");
	if (type_ == AV_HWDEVICE_TYPE_NONE)
	{
		cout << "type error check if u run it on windows \n";
	}
	if (avformat_open_input(&av_fmt_, path, NULL, NULL) != 0)
	{
		cout << "open input failure \n";
		return false;
	}
	if (avformat_find_stream_info(av_fmt_, NULL) < 0)
	{
		cout << "find stream info failure \n";
		return false;
	}
	//先处理视频流
	video_index_ = av_find_best_stream(av_fmt_, AVMEDIA_TYPE_VIDEO, -1, -1, &av_video_codec_, 0);
	if (video_index_ < 0)
	{
		cout << "find video stream failure \n";
	}
	else
	{
		video_exist_ = true;
		//遍历这个编解码器支持的所有硬件解码类型
		for (int i = 0;; i++)
		{
			const AVCodecHWConfig* config = avcodec_get_hw_config(av_video_codec_, i);
			if (!config)
			{
				cout << "Decoder does not support this device type \n";
				return false;
			}
			if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX && config->device_type == type_)
			{
				//把硬件支持的像素格式设置进去
				//GPU 操作系统之间有很复杂的关系 很麻烦说老实话
				hw_pix_fmt_ = config->pix_fmt;
				break;
			}
		}
		if (!(av_video_codec_ctx_ = avcodec_alloc_context3(av_video_codec_)))
		{
			cout << "alloc_codec_ctx failure \n";
			return false;
		}
		av_video_stream_ = av_fmt_->streams[video_index_];
		//解码器上下文设置好
		if (avcodec_parameters_to_context(av_video_codec_ctx_, av_video_stream_->codecpar) < 0)
		{
			cout << "to context failure \n";
			return false;
		}
		av_video_codec_ctx_->get_format = &DecodeDemuxPipeLine::GetHWFormat;
		if (av_hwdevice_ctx_create(&hw_device_ctx_, type_, NULL, NULL, 0) < 0)
		{
			cout << "create specified HW ctx failure \n";
			return false;
		}
		av_video_codec_ctx_->hw_device_ctx = hw_device_ctx_;
		//打开解码器
		if (avcodec_open2(av_video_codec_ctx_, av_video_codec_, NULL) < 0)
		{
			cout << "fail to open codec \n";
			return false;
		}
	}
	//处理音频流
	audio_index_ = av_find_best_stream(av_fmt_,AVMEDIA_TYPE_AUDIO,0,0,&av_audio_codec_,0);
	if (audio_index_ < 0)
	{
		cout << "does not find audio stream \n";
	}
	else
	{
		audio_exist_ = true;
		av_audio_stream_ = av_fmt_->streams[audio_index_];
		//因为上文如果找到了流 那么我们默认已经找到了编解码器了
		av_audio_codec_ctx_ = avcodec_alloc_context3(av_audio_codec_);
		if (!av_audio_codec_ctx_)
		{
			cout << "alloc failure \n";
			return false;
		}
		if (avcodec_parameters_to_context(av_audio_codec_ctx_, av_audio_stream_->codecpar) < 0)
		{
			cout << "copy parameters failure \n";
			return false;
		}
		if (avcodec_open2(av_audio_codec_ctx_, av_audio_codec_, NULL) < 0)
		{
			cout << "open audio codec failure \n";
		}
	}
}

int DecodeDemuxPipeLine::InterruputCallBack(void* ctx)
{
	DecodeDemuxPipeLine* tmp = (DecodeDemuxPipeLine*)ctx;

	return 0;
}
enum AVPixelFormat DecodeDemuxPipeLine::GetHWFormat(AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts)
{
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++) {
		if (*p == hw_pix_fmt_)
			return *p;
	}

	fprintf(stderr, "Failed to get HW surface format.\n");
	return AV_PIX_FMT_NONE;
}