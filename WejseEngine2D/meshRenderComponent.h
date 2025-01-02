#pragma once

#include "shader_s.h"

struct MeshRenderComponent
{
	std::unique_ptr<Shader> shader;

    std::string shapeName ;
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
};