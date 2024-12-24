#pragma once
#include "RenderComponent.h"

#include "WejseRenderer.h"
#include "Registry.h"

void RenderInitialise();


void RenderUpdate(Registry& registry);
void updateTexture(const std::string& newTexturePath, EntityRegistry::Entity entity);
void RenderCleanUp();
