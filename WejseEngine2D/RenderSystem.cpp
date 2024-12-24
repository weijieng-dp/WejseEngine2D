#include "RenderSystem.h"
#include "WejseRenderer.h"
#include "Registry.h"

unsigned int VAO;
unsigned int EBO;
unsigned int VBO;



void RenderInitialise()
{
    initialiseOpenGL(VBO,VAO,EBO);


}

void RenderUpdate(Registry& registry)
{
    glm::mat4 viewMatrix = glm::mat4(1.0);
    glm::mat4 projectionMatrix = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);
    // Retrieve all entities with a RenderComponent
    auto entitiesWithRender = registry.getEntitiesWithComponent<RenderComponent>();
    for (auto& entity : entitiesWithRender) {
        auto* renderComp = registry.getComponent<RenderComponent>(entity);
        auto* transformComp = registry.getComponent<TransformComponent>(entity);

        // Set matrices in the shader
        renderComp->shader->use();
        renderComp->shader->setMat4("model", transformComp->transform);
        renderComp->shader->setMat4("view", viewMatrix);
        renderComp->shader->setMat4("projection", projectionMatrix);

        renderComp->tex->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

}

void updateTexture(const std::string& newTexturePath, EntityRegistry::Entity entity) {
    Registry& registry = Registry::instance();
    auto rendercomp = registry.getComponent<RenderComponent>(entity);

    rendercomp->tex = std::make_unique<texture>(("Assets/" + newTexturePath).c_str()); // Load the new texture
    rendercomp->TextureString = newTexturePath;
}

void RenderCleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
