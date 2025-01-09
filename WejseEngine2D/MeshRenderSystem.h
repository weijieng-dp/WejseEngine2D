#pragma once
#include "Registry.h"
#include "meshRenderComponent.h"
#include "WejseRenderer.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Camera.h"
#include "imgui.h"

class MeshRenderSystem
{
public:
	void meshRenderInitialisation();
	void meshRenderUpdate();
	void meshRenderRender();
	void meshRenderCleanUp();
private:
	Registry& registry = Registry::instance();
};

