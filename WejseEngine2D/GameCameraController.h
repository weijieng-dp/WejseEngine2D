#include "Registry.h"
#include "ComponentFile.h"
#include "WejseRenderer.h"

static Registry& reg = Registry::instance();

void GameCameraController()
{
//	auto GameCameraEntity = reg.getEntitiesWithComponent<CameraControlComponent>();
//	auto PlayerEntity = reg.getEntitiesWithComponent<PlayerComponent>();
//	for (auto CameraEntity : GameCameraEntity)
//	{
//		auto cameraComp = registry.getComponent<CameraControlComponent>(CameraEntity);
//		auto cameraTransformComp = registry.getComponent<TransformComponent>(CameraEntity);
//
//		for (auto TrackingEntity : PlayerEntity)
//		{
//			auto playerComp = registry.getComponent<PlayerComponent>(TrackingEntity);
//			auto playerTransformComp = registry.getComponent<TransformComponent>(TrackingEntity);
//
//			cameraTransformComp->translate.y = -(playerTransformComp->translate.y);
//		}
//	} 
//}

//void GameCameraController()
//{
//    auto GameCameraEntity = reg.getEntitiesWithComponent<CameraControlComponent>();
//    auto PlayerEntity = reg.getEntitiesWithComponent<PlayerComponent>();
//
//    for (auto CameraEntity : GameCameraEntity) {
//        auto cameraComp = registry.getComponent<CameraComponent>(CameraEntity);
//        auto cameraTransformComp = registry.getComponent<TransformComponent>(CameraEntity);
//
//        for (auto TrackingEntity : PlayerEntity) {
//            auto playerComp = registry.getComponent<PlayerComponent>(TrackingEntity);
//            auto playerTransformComp = registry.getComponent<TransformComponent>(TrackingEntity);
//
//            // Get camera's half height
//            float cameraHalfHeight = cameraComp.camera.getHeight() / 2.0f;
//
//            // Calculate the desired camera Y position based on player's position
//            float targetCameraY = -playerTransformComp->translate.y;
//
//            // If the player is above the camera's midpoint, move the camera up
//            if (playerTransformComp->translate.y > cameraTransformComp->translate.y + cameraHalfHeight) {
//                targetCameraY += cameraHalfHeight;
//            }
//
//            // Smoothly move the camera towards the target position
//            cameraTransformComp->translate.y = glm::lerp(cameraTransformComp->translate.y, targetCameraY, 0.1f);
//        }
//    }
}