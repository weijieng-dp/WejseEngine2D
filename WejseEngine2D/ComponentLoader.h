#pragma once
#include "ComponentRegistry.h"
#include "Registry.h"

#include "TransformComponent.h"
#include "SpriteRenderComponent.h"
#include "selectionComponent.h"
#include "meshRenderComponent.h"




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
			registry.addComponent<selectionComponent>(entity, {false});
		});

	ComponentRegistry.registerComponent<MeshRenderComponent>(
		"Mesh Render Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<MeshRenderComponent>(entity, std::move(MeshRenderComponent("shader/shader.vs", "shader/shader.fs")));
		});
}

