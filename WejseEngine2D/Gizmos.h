
#pragma once

#include "imgui.h"
#include "Camera.h"
#include "WejseRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Gizmos
{
public:

	Gizmos() {
		AxisX = registry.createEntity();


		registry.addComponent<TransformComponent>(AxisX, {});
		//registry.addComponent<MeshRenderComponent>(AxisX, std::move(MeshRenderComponent("shader/shader.vs", "shader/shader.fs", glm::vec3(1, 0, 0), "triangle")));
	}

	void SetGizmosOnEntity(EntityRegistry::Entity ent)
	{
		auto targettransformComp = registry.getComponent<TransformComponent>(ent);
		auto axisXtransformcomp = registry.getComponent<TransformComponent>(AxisX);
		if (targettransformComp)
		{
			axisXtransformcomp->translate = glm::vec3(targettransformComp->translate.x, targettransformComp->translate.y + 50, targettransformComp->translate.z);
			axisXtransformcomp->rotation = 0;
			axisXtransformcomp->scale = glm::vec3(100, 300, 100);
		}


	}



private:

	Registry& registry = Registry::instance();
	EntityRegistry::Entity AxisX;

};