
#include "SpriteRenderSystem.h"


camera cam;

void RenderInitialise()
{
    Registry& registry = Registry::instance();
    initialiseQuad();


}

void RenderUpdate(Registry& registry)
{
    if(glfwGetMouseButton(window,1) == GLFW_PRESS)
        cam.processMovement();
    
    glm::mat4 viewMatrix = cam.getviewmatrix();
    glm::mat4 projectionMatrix = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);
    // Retrieve all entities with a SpriteRenderComponent
    auto entitiesWithRender = registry.getEntitiesWithComponent<SpriteRenderComponent>();
    for (auto& entity : entitiesWithRender) {
        auto* renderComp = registry.getComponent<SpriteRenderComponent>(entity);
        auto* transformComp = registry.getComponent<TransformComponent>(entity);




        // Set matrices in the shader
        renderComp->shader->use();
        renderComp->shader->setMat4("model", transformComp->transform);
        renderComp->shader->setMat4("view", viewMatrix);
        renderComp->shader->setMat4("projection", projectionMatrix);

        renderComp->shader->setVec3("uColor", renderComp->color);
        renderComp->shader->setBool("hasTexture", true);

        renderComp->tex->use();

        
        DrawQuads();

    }
}

void updateTexture(const std::string& newTexturePath, EntityRegistry::Entity entity) {
    Registry& registry = Registry::instance();
    auto rendercomp = registry.getComponent<SpriteRenderComponent>(entity);

    auto tex = std::make_unique<texture>((newTexturePath).c_str()); // Load the new texture

    if (!tex->isValid())
    {
        rendercomp->hasTexture = false;
    }
    else
    {
        rendercomp->hasTexture = true;
        rendercomp->tex = std::make_unique<texture>((newTexturePath).c_str()); // Load the new texture
        rendercomp->TextureString = newTexturePath;

    }
}
