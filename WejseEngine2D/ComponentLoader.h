#pragma once
#include "ComponentRegistry.h"
#include "Registry.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "selectionComponent.h"




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

	ComponentRegistry.registerComponent<RenderComponent>(
		"Render Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<RenderComponent>(entity, std::move(RenderComponent("shader/shader.vs", "shader/shader.fs", "Assets/PlanetTexture.png")));
		});


	ComponentRegistry.registerComponent<selectionComponent>(
		"Selection Component",
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<selectionComponent>(entity, {false});
		});
}

