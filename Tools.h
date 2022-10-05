#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include<boost/lockfree/queue.hpp>
#include<libavcodec/avcodec.h>
#include<mutex>
#include<condition_variable>
#define cout std::cout<<__FILE__<<":"<<__LINE__<<":"
class GlobalTools
{
public:
	static boost::lockfree::queue<AVFrame*> g_video_wait_for_render_queue;
	static boost::lockfree::queue<AVFrame*> g_audio_wait_for_display_queue;
	static boost::lockfree::queue<AVPacket*> g_video_wait_for_decode_queue;
	static boost::lockfree::queue<AVPacket*> g_audio_wait_for_decode_queue;
	static GLFWwindow* InitAndCreateWindow();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

