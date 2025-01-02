#pragma once
#include "component.h"
#include "shader_s.h"
#include "texture_s.h"
#include "TransformSystem.h"


struct SpriteRenderComponent {
	std::unique_ptr<Shader> shader;
	std::unique_ptr<texture> tex;
    std::string TextureString;
    glm::vec3 color = glm::vec3(1.0,1.0,1.0);
    bool hasTexture = true;

    SpriteRenderComponent() = default;

    // Constructor
    SpriteRenderComponent(const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath, glm::vec3 color = glm::vec3(1.0,1.0,1.0))
        : shader(std::make_unique<Shader>(vertexPath.c_str(), fragmentPath.c_str())),
        tex(std::make_unique<texture>(texturePath.c_str())), TextureString(texturePath), color(color){}

    // Delete copy constructor and copy assignment operator
    SpriteRenderComponent(const SpriteRenderComponent&) = delete;
    SpriteRenderComponent& operator=(const SpriteRenderComponent&) = delete;

    // Default move constructor and move assignment operator
    SpriteRenderComponent(SpriteRenderComponent&&) noexcept = default;
    SpriteRenderComponent& operator=(SpriteRenderComponent&&) noexcept = default;
};

