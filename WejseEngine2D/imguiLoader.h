#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ContentBrowser.h"
#include "EntityPanel.h"
#include "InspectorPanel.h"
#include "ScenePanel.h"
#include "meshRenderSystem.h"

void InitializeImGui(GLFWwindow* window);
void UpdateImGui();
void RenderImGui();
void CleanupImGui();


