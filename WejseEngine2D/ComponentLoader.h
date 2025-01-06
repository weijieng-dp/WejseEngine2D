#pragma once
#include "ComponentRegistry.h"
#include "Registry.h"

#include "TransformComponent.h"
#include "SpriteRenderComponent.h"
#include "selectionComponent.h"
#include "meshRenderComponent.h"
#include "CameraComponent.h"

#include <rttr/registration>




inline void ComponentInitialise()
{

	Registry& registry = Registry::instance();
	componentRegistry& ComponentRegistry = componentRegistry::instance();


	// Register components
	ComponentRegistry.registerComponent<TransformComponent>(
		"Transform Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<TransformComponent>(entity, {});
			
		});

		

	ComponentRegistry.registerComponent<SpriteRenderComponent>(
		"Sprite Render Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<SpriteRenderComponent>(entity, std::move(SpriteRenderComponent("shader/shader.vs", "shader/shader.fs", "Assets/PlanetTexture.png")));
		});


	ComponentRegistry.registerComponent<selectionComponent>(
		"Selection Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<selectionComponent>(entity, { false });
		});

	ComponentRegistry.registerComponent<MeshRenderComponent>(
		"Mesh Render Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<MeshRenderComponent>(entity, std::move(MeshRenderComponent("shader/shader.vs", "shader/shader.fs")));
		});

	ComponentRegistry.registerComponent<CameraComponent>(
		"Camera Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<CameraComponent>(entity, { });
		});
}

RTTR_REGISTRATION
{
	rttr::registration::class_<TransformComponent>("Transform Component")
		.constructor<>()
		.property("Translate", &TransformComponent::translate)
		.property("Rotation", &TransformComponent::rotation)
		.property("Scale", &TransformComponent::scale);

	rttr::registration::class_<MeshRenderComponent>("Mesh Render Component")
		.constructor<>()
		.property("color", &MeshRenderComponent::color)
		.property("shape", &MeshRenderComponent::shapeName);

	rttr::registration::class_<SpriteRenderComponent>("Sprite Render Component")
		.constructor<>()
		.property("color", &SpriteRenderComponent::color)
		.property("Texture", &SpriteRenderComponent::TextureString);

	rttr::registration::class_<CameraComponent>("Camera Component");

	rttr::registration::class_<LayerComponent>("Layer Component")
		.constructor<>()
		.property("layer", &LayerComponent::layer);
}
