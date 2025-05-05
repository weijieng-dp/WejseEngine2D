#pragma once
//---------------------------------------------------------
// @file RenderSystem.h
// @project name: Book-It
// @authors Ng wei jie (weijie.ng)
// @brief This file declares function that handles rendering shapes (quads, triangles, circles, lines),
// textures, and fonts using OpenGL. It manages VAOs, VBOs, shaders, and texture updates
// while integrating with an ECS. It also supports lighting, camera transformations,
// and font rendering with FreeType. Cleanup functions ensure proper resource deallocation.
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "WejseRenderer.h"
#include "Registry.h"
#include "Camera.h"
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H	
#include "ComponentFile.h"

void renderInitialise();
void meshRenderInitialisation();

void renderUpdate(Registry& registry);
void updateTexture(const std::string& newTexturePath, EntityRegistry::Entity entity);

void meshRenderUpdate();
void meshRenderRender();

void meshRenderCleanUp();

void RenderCleanUp();

void fontRenderer();
void fontInitialise(std::string fontName, EntityRegistry::Entity ent);
void fontUpdate(std::string fontName, EntityRegistry::Entity ent);
void fontCleanUp();