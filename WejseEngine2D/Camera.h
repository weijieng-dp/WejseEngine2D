#pragma once
//---------------------------------------------------------
// @file camera.h
// @project name: Book-It
// @authors Ng wei jie (weijie.ng)
// @brief This file declares function that get the view matrix,
// and process movement as well as find entity
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ComponentFile.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "Registry.h"

class camera
{
public:
	static camera& instance() {
		static camera instance;  // Created lazily
		return instance;
	}

	// Prevent copying and assignment
	camera(const camera&) = delete;
	camera& operator=(const camera&) = delete;

	glm::mat4 getviewmatrix();

	void processMovement();
	void FindEntity(Registry& reg, EntityRegistry::Entity ent);
private:
	camera() {}
	glm::mat4 camPos = glm::mat4(1.0);
	glm::vec3 position = glm::vec3(0,0,0);
	float speed = 5;

};