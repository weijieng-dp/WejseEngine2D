#pragma once
#include "component.h"
#include "shader_s.h"
#include "texture_s.h"
#include "TransformSystem.h"


struct RenderComponent {
	std::unique_ptr<Shader> shader;
	std::unique_ptr<texture> tex;
    std::string TextureString;


    RenderComponent() = default;

    // Constructor
    RenderComponent(const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath)
        : shader(std::make_unique<Shader>(vertexPath.c_str(), fragmentPath.c_str())),
        tex(std::make_unique<texture>(texturePath.c_str())), TextureString(texturePath){}

    // Delete copy constructor and copy assignment operator
    RenderComponent(const RenderComponent&) = delete;
    RenderComponent& operator=(const RenderComponent&) = delete;

    // Default move constructor and move assignment operator
    RenderComponent(RenderComponent&&) noexcept = default;
    RenderComponent& operator=(RenderComponent&&) noexcept = default;
};

