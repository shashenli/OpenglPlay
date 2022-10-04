#include "DecodeDemuxPipeLine.h"
std::mutex DecodeDemuxPipeLine::mtx_for_create_instance_{};
std::weak_ptr<DecodeDemuxPipeLine> DecodeDemuxPipeLine::instance_{};
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

void DecodeDemuxPipeLine::Init(const char* path)
{
	av_fmt_ = avformat_alloc_context();
	//注册网络流超时回调
	av_fmt_->interrupt_callback.callback = &DecodeDemuxPipeLine::InterruputCallBack;
	av_fmt_->interrupt_callback.opaque = this;
	

}

int DecodeDemuxPipeLine::InterruputCallBack(void* ctx)
{
	DecodeDemuxPipeLine* tmp = (DecodeDemuxPipeLine*)ctx;

	return 0;
}
