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
	void Init(const char* path);
	void StartDecode();
private:
	static int InterruputCallBack(void*ctx);
private:
	static std::mutex mtx_for_create_instance_;
	static std::weak_ptr<DecodeDemuxPipeLine> instance_;
	std::thread read_pkt_thread_;
	std::thread decode_video_pkt_thread_;
	std::thread decode_audio_pkt_thread_;
	AVFormatContext* av_fmt_;
	AVPacket* av_pkt_;
	AVFrame* av_frame_;
	AVCodec* av_video_codec_;
	AVCodec* av_audio_codec_;
	int abort_request_;
};

