#include "Tools.h"
GLFWwindow* Tools::InitAndCreateWindow()
{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return nullptr;
		}
	return window;
}

void Tools::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
		glViewport(0, 0, width, height);
}
