#include "GraphicRenderPipeLine.h"
std::weak_ptr<GraphicRenderPipeLine> GraphicRenderPipeLine::instance_{};

GraphicRenderPipeLine::GraphicRenderPipeLinePtr GraphicRenderPipeLine::CreateInstance()
{
	std::lock_guard<std::mutex> lock_guard(mtx_for_create_instance_);
	if (1 == instance_.expired())
	{
		std::shared_ptr<GraphicRenderPipeLine>tmp(new GraphicRenderPipeLine());
		instance_ = tmp;
		return tmp;
	}
	else
	{
		return instance_.lock();
	}
}
GraphicRenderPipeLine::GraphicRenderPipeLine()
{
	window_handle_ = GlobalTools::InitAndCreateWindow();
	render_thread_ = std::thread([window_handle=this->window_handle_]()
		{
		//绑定opengl上下文至当前线程和当前窗口 每个窗口可以有多个opengl上下文 但每个线程最多只有一个opengl上下文
		glfwMakeContextCurrent(window_handle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		//设置窗口渲染窗口大小
		glViewport(0, 0, 800, 600);
		//设置窗口大小发送改变的回调
		glfwSetFramebufferSizeCallback(window_handle, GlobalTools::framebuffer_size_callback);
		//开始渲染循环
		while (!glfwWindowShouldClose(window_handle))
		{
			glfwSwapBuffers(window_handle);
			glfwPollEvents();
		}
		glfwTerminate();
		});
}

GraphicRenderPipeLine::~GraphicRenderPipeLine()
{
	if (render_thread_.joinable())
	{
		render_thread_.join();
	}
}
