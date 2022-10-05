#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
}
#include<memory>
#include<mutex>
#include<thread>
#include"Tools.h"
class DecodeDemuxPipeLine
{
	using DecodeDemuxPipeLinePtr = std::shared_ptr<DecodeDemuxPipeLine>;
public:
	static DecodeDemuxPipeLinePtr CreatInstance();
	bool Init(const char* path);
	void StartDecode();
private:
	static int InterruputCallBack(void*ctx);

	static enum AVPixelFormat GetHWFormat(AVCodecContext* ctx,
		const enum AVPixelFormat* pix_fmts);
private:
	static std::mutex mtx_for_create_instance_;
	static std::weak_ptr<DecodeDemuxPipeLine> instance_;
	//使用智能指针来封装普通数据类型我觉得是一种很愚蠢的行为 特别是对于你调用的c库来说
	std::thread read_pkt_thread_{};
	std::thread decode_video_pkt_thread_{};
	std::thread decode_audio_pkt_thread_{};
	AVFormatContext* av_fmt_{};
	AVPacket* av_pkt_{};
	AVFrame* av_frame_{};
	AVCodecContext* av_video_codec_ctx_;
	AVCodec* av_video_codec_{};
	AVCodecContext* av_audio_codec_ctx_;
	AVCodec* av_audio_codec_{};
	int video_index_{};
	int audio_index_{};
	int abort_request_{};
	enum AVHWDeviceType type_ {};
	static enum AVPixelFormat hw_pix_fmt_;
	AVStream* av_video_stream_{};
	AVStream* av_audio_stream_{};
	AVBufferRef* hw_device_ctx_{};
	bool video_exist_{};
	bool audio_exist_{};
};

