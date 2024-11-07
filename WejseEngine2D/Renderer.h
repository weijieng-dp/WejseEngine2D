#pragma once
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <iostream>

class Renderer
{
public:
	int initializeRendering();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
private:
};

