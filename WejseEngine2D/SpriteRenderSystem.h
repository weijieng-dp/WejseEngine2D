#pragma once
#include "SpriteRenderComponent.h"
#include "WejseRenderer.h"
#include "Registry.h"
#include "Camera.h"

void RenderInitialise();


void RenderUpdate(Registry& registry);
void updateTexture(const std::string& newTexturePath, EntityRegistry::Entity entity);
