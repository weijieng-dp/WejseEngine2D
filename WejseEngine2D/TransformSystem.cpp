//---------------------------------------------------------
// @file TransformSystem.cpp
// @authors Ng wei jie (weijie.ng)
// @brief This file defines function that updates transfrom based on the anchor points
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "TransformSystem.h"


void UpdateTransform(Registry& registry)
{
    auto entitiesWithTransform = registry.getEntitiesWithComponent<TransformComponent>();
    for (auto entity : entitiesWithTransform) {
        auto transform = registry.getComponent<TransformComponent>(entity);
        auto anchor = registry.getComponent<AnchorComponent>(entity);

        // Update the transform (position, rotation, etc.)
        if (transform && anchor)
        {
            transform->transform = glm::mat4(1.0);

            if (anchor->AnchoredTo == "Center")
            {
                transform->transform = glm::translate(transform->transform, transform->translate);
                transform->transform = glm::rotate(transform->transform, -glm::radians(transform->rotation), transform->rotate);
                transform->transform = glm::scale(transform->transform, transform->scale);
            }
            else if (anchor->AnchoredTo == "Left")
            {

                transform->transform = glm::translate(transform->transform, transform->translate + glm::vec3(transform->scale.x / 2, 0, 0));
                transform->transform = glm::rotate(transform->transform, -glm::radians(transform->rotation), transform->rotate);
                transform->transform = glm::scale(transform->transform, transform->scale);

            }
            else if (anchor->AnchoredTo == "Right")
            {
                transform->transform = glm::translate(transform->transform, transform->translate - glm::vec3(transform->scale.x/2, 0, 0));
                transform->transform = glm::rotate(transform->transform, -glm::radians(transform->rotation), transform->rotate);
                transform->transform = glm::scale(transform->transform, transform->scale);
            }
        }
    }
}