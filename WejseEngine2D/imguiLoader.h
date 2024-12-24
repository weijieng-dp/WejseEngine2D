#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

	void InitializeImGui(GLFWwindow* window);
	void UpdateImGui();
	void RenderImGui();
	void CleanupImGui();
	void unbind_framebuffer();
	void bind_framebuffer();
	void create_framebuffer();
	void rescale_framebuffer(float width, float height);
	void SetImGuiFullScreen(GLFWwindow* window);


