#pragma once
#define GLM_ENABLE_EXPERIMENTAL

//---------------------------------------------------------
// @file imguiLoader.h
// @project name: Book-It
// @authors Ng wei jie (weijie.ng)
// 
// @brief This file declare function that
// implements an ImGui-based UI for a 3D scene and entity management
// system using OpenGL. It includes panels for entity selection, manipulation
// (move, rotate, scale), and scene rendering with ImGuizmo. Entity copy/paste
// and drag-and-drop functionality are supported. Framebuffers are used for scene
// rendering and object picking. A content browser panel allows navigation and asset dragging.
// Scene loading and saving are handled, with real-time scene updates. ImGui is initialized,
// and its rendering is integrated with OpenGL. The system supports responsive UI resizing and
// dynamic entity transformations, providing a comprehensive tool for scene and asset management.
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <filesystem>
#include "SceneManager.h"
#include "TransformSystem.h"
#include "ImGuizmo.h"
#include "Camera.h"
#include <glm/gtx/matrix_decompose.hpp>


void initializeImGui(GLFWwindow* window);
void updateImGui();
void renderImGui();
void cleanupImGui();


