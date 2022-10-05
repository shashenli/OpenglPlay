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
		//��opengl����������ǰ�̺߳͵�ǰ���� ÿ�����ڿ����ж��opengl������ ��ÿ���߳����ֻ��һ��opengl������
		glfwMakeContextCurrent(window_handle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		//���ô�����Ⱦ���ڴ�С
		glViewport(0, 0, 800, 600);
		//���ô��ڴ�С���͸ı�Ļص�
		glfwSetFramebufferSizeCallback(window_handle, GlobalTools::framebuffer_size_callback);
		//��ʼ��Ⱦѭ��
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
