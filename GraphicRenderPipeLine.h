#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <memory>
#include"Tools.h"
#include<thread>
#include<mutex>
class GraphicRenderPipeLine:std::enable_shared_from_this<GraphicRenderPipeLine>
{
	using GraphicRenderPipeLinePtr = std::shared_ptr<GraphicRenderPipeLine>;
public:
	static GraphicRenderPipeLinePtr CreateInstance();
	~GraphicRenderPipeLine();
private:
	GraphicRenderPipeLine();
	GraphicRenderPipeLine operator=(const GraphicRenderPipeLine& e) = delete;
	GraphicRenderPipeLine(const GraphicRenderPipeLine& e) = delete;
private:
	static std::mutex mtx_for_create_instance_;
	static std::weak_ptr<GraphicRenderPipeLine> instance_;
	GLFWwindow* window_handle_;
	std::thread render_thread_;
};

