
//---------------------------------------------------------
// @file camera.cpp
// @project name: Book-It
// @authors Ng wei jie (weijie.ng)
// @brief This file declares function that get the view matrix,
// and process movement as well as find entity
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "Camera.h"


glm::mat4 camera::getviewmatrix()
{
	camPos = glm::mat4(1);
	return glm::translate(camPos, position);
}

void camera::processMovement()
{
	static ImVec2 oldPositions = { 0,0 };

	ImVec2 mousePos = ImGui::GetMousePos();
	if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		oldPositions = mousePos;
		return;
	}

	if (!ImGui::IsWindowFocused()) return;


	if (oldPositions.x == 0 && oldPositions.y == 0) {
		oldPositions = mousePos;
	}

	position += glm::vec3(mousePos.x - oldPositions.x, -(mousePos.y - oldPositions.y), 0);

	oldPositions = mousePos; // Update for next frame
}




void camera::FindEntity(Registry& reg,EntityRegistry::Entity ent)
{
	auto TransformComp = reg.getComponent<TransformComponent>(ent);

	if (TransformComp)
	{
		position = -TransformComp->translate;
		std::cout << position.x << "," << position.y << std::endl;
	}
}
