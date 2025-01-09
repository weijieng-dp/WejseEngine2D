#include "TransformSystem.h"


void UpdateTransform(Registry& registry)
{
    auto entitiesWithTransform = registry.getEntitiesWithComponent<TransformComponent>();
    for (auto entity : entitiesWithTransform) {
        auto transform = registry.getComponent<TransformComponent>(entity);

        // Update the transform (position, rotation, etc.)
        if (transform)
        {
            transform->transform = glm::mat4(1.0);
            transform->transform = glm::translate(transform->transform, transform->translate);
            transform->transform = glm::rotate(transform->transform, -glm::radians(transform->rotation), transform->rotate);
            transform->transform = glm::scale(transform->transform, transform->scale);
        }
    }
}