#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
class Tools
{
public:
	static GLFWwindow* InitAndCreateWindow();
	
private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

