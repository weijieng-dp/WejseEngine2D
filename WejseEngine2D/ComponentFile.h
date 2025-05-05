#pragma once

//---------------------------------------------------------
// @file ComponentFile.h
// @project name: Book-It
// @authors Ng wei jie (weijie.ng),Seow Sin Le (s.sinle@digipen.edu), Zhang Mingyang (mingyang.zhang),
// Tengku Danish Ashraf Bin Tengku Azlan (t.bintengkuazlan)
// 
// @brief contain all component for ECS
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//--------------------------------------------------------- 

#include <string>
#include "shader_s.h"
#include "texture_s.h"
#include "rttr/rttr_enable.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ScriptingAPI.h"

struct NameComponent
{
    std::string name = "Entities";
};

struct AnchorComponent
{
	std::string AnchoredTo = "Center";
};


struct CameraComponent: Script
{
     void OnStart()   { } 
     void OnUpdate() {  }
    RTTR_ENABLE(Script)
};


//not used for rttr
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

struct textComponent : Script
{
    std::unique_ptr<Shader> shader;
    std::string text;
    std::string font;
    glm::vec3 color = glm::vec3(1,1,1);
    std::map<GLchar, Character> Characters;


    textComponent() :shader(std::make_unique<Shader>("shader/WordText.vs", "shader/WordText.fs")) {};
    RTTR_ENABLE(Script)

};

struct MeshRenderComponent : Script
{
    std::unique_ptr<Shader> shader;

    std::string shapeName;
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
    bool hasTexture = true;

    MeshRenderComponent() = default;

    // Constructor
    MeshRenderComponent(const std::string& vertexPath, const std::string& fragmentPath, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0), std::string shapeName = "square")
        : shader(std::make_unique<Shader>(vertexPath.c_str(), fragmentPath.c_str()))
        , color(color), shapeName(shapeName) {}

    // Delete copy constructor and copy assignment operator
    MeshRenderComponent(const MeshRenderComponent&) = delete;
    MeshRenderComponent& operator=(const MeshRenderComponent&) = delete;

    // Default move constructor and move assignment operator
    MeshRenderComponent(MeshRenderComponent&&) noexcept = default;
    MeshRenderComponent& operator=(MeshRenderComponent&&) noexcept = default;

    RTTR_ENABLE(Script)
};

struct selectionComponent
{
    bool selected = false;
};

struct SpriteRenderComponent : Script
{
    std::unique_ptr<Shader> shader;

    std::unique_ptr<texture> tex;
    std::string TextureString;
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
    bool hasTexture = true;

    SpriteRenderComponent() = default;

    // Constructor
    SpriteRenderComponent(const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0))
        : shader(std::make_unique<Shader>(vertexPath.c_str(), fragmentPath.c_str())),
        tex(std::make_unique<texture>(texturePath.c_str())), TextureString(texturePath), color(color) {}

    // Delete copy constructor and copy assignment operator
    SpriteRenderComponent(const SpriteRenderComponent&) = delete;
    SpriteRenderComponent& operator=(const SpriteRenderComponent&) = delete;

    // Default move constructor and move assignment operator
    SpriteRenderComponent(SpriteRenderComponent&&) noexcept = default;
    SpriteRenderComponent& operator=(SpriteRenderComponent&&) noexcept = default;

    RTTR_ENABLE(Script)
};

struct TransformComponent
{
    glm::vec3 translate = glm::vec3(0, 0, 0);
    float rotation = 0;
    glm::vec3 scale = glm::vec3(250, 250, 1);
    glm::vec3 rotate = glm::vec3(0, 0, 1.0f);

    glm::mat4 transform = glm::mat4(1.0f);

};

struct lightComponent : Script
{
    lightComponent() : lightIntensity(1.0f) {}
    float lightIntensity = 1.0f;
    RTTR_ENABLE(Script)

};

struct ActiveComponent
{
    bool isActive = true;
};

