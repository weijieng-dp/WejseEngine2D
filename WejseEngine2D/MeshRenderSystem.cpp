#include "MeshRenderSystem.h"


void MeshRenderSystem::meshRenderInitialisation()
{
    initialiseWireFrameBox();
    initialiseCircle();
    initialiseTriangle();
    initialiseLines();
    initialiseSquare();
}

void MeshRenderSystem::meshRenderUpdate()
{

}

void MeshRenderSystem::meshRenderRender()
{
    camera& cam = camera::instance();

    glm::mat4 viewMatrix;
    if (debug)
    {
        viewMatrix = cam.getviewmatrix();


        if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
            cam.processMovement();
    }
    else
    {
        auto entitywithcameracomp = registry.getEntitiesWithComponents<CameraComponent>();
        for (auto entity : entitywithcameracomp)
        {
            auto transformcomp = registry.getComponent<TransformComponent>(entity);
            viewMatrix = transformcomp->transform;
        }
    }
    glm::mat4 projectionMatrix = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);
    auto entitywithMeshrendercomp = registry.getEntitiesWithComponent<MeshRenderComponent>();
    for (auto entity : entitywithMeshrendercomp)
    {
        auto meshRenderComp = registry.getComponent<MeshRenderComponent>(entity);
        auto transformComp = registry.getComponent<TransformComponent>(entity);

        
        meshRenderComp->shader->use();
        meshRenderComp->shader->setMat4("model", transformComp->transform);
        meshRenderComp->shader->setMat4("view", viewMatrix);
        meshRenderComp->shader->setMat4("projection", projectionMatrix);

        meshRenderComp->shader->setVec3("uColor", meshRenderComp->color);
        meshRenderComp->shader->setBool("hasTexture", false);
        if (meshRenderComp->shapeName == "square")
        {
            drawsquare();
        }
        if (meshRenderComp->shapeName == "triangle")
        {
            DrawTriangle();
        }
        if (meshRenderComp->shapeName == "circle")
        {
            drawfilledCircle();
        }
        if (meshRenderComp->shapeName == "line")
        {
            drawLine();
        }
    }
}

void MeshRenderSystem::meshRenderCleanUp()
{

}