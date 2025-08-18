#pragma once
#include "Registry.h"

#include "ComponentFile.h"
#define RTTR_DLL
#include <rttr/registration>
#include <rttr/argument.h>

template<typename T>
void registerComp()
{
	Registry& registry = Registry::instance();
	componentRegistry& ComponentRegistry = componentRegistry::instance();

	if (typeid(T) == typeid(MeshRenderComponent))
	{
		ComponentRegistry.registerComponent<MeshRenderComponent>(
			rttr::type::get<MeshRenderComponent>().get_name().to_string(),
			[&registry](EntityRegistry::Entity entity) {
				registry.addComponent<MeshRenderComponent>(entity, std::move(MeshRenderComponent("shader/shader.vs", "shader/shader.fs")));

			});

		return;
	}
	if (typeid(T) == typeid(SpriteRenderComponent))
	{
		ComponentRegistry.registerComponent<SpriteRenderComponent>(
			rttr::type::get<SpriteRenderComponent>().get_name().to_string(),
			[&registry](EntityRegistry::Entity entity) {
				registry.addComponent<SpriteRenderComponent>(entity, std::move(SpriteRenderComponent("shader/shader.vs", "shader/shader.fs", "Assets/PlanetTexture.png")));
			});

		return;
	}
	ComponentRegistry.registerComponent<T>(
		rttr::type::get<T>().get_name().to_string(),
		[&registry](EntityRegistry::Entity entity) {
			registry.addComponent<T>(entity, {});

		});


};

        void ComponentInitialise()
        {
	        // Register components
	        registerComp<TransformComponent>();
	        registerComp<AnchorComponent>();
	        registerComp<ActiveComponent>();
	        registerComp<NameComponent>();
	        registerComp<SpriteRenderComponent>();
	        registerComp<selectionComponent>();
	        registerComp<MeshRenderComponent>();
	        registerComp<CameraComponent>();
	        registerComp<textComponent>();
	        registerComp<lightComponent>();

}
RTTR_REGISTRATION
{

		rttr::registration::class_<TransformComponent>("Transform Component")
		.constructor<>()
		.property("Translate", &TransformComponent::translate)
		.property("Rotation", &TransformComponent::rotation)
		.property("Scale", &TransformComponent::scale);

	rttr::registration::class_<selectionComponent>("Selection Component")
		.constructor<>(rttr::registration::private_access);

	

	rttr::registration::class_<AnchorComponent>("Anchor Component")
		.constructor<>()
		.property("AnchoredTo", &AnchorComponent::AnchoredTo);

	rttr::registration::class_<CameraComponent>("Camera Component")
		.constructor<>()
		.method("OnStart", &CameraComponent::OnStart)
		.method("OnUpdate", &CameraComponent::OnUpdate);

	rttr::registration::class_<ActiveComponent>("Active Component")
		.constructor<>()
		.property("Active", &ActiveComponent::isActive);

	rttr::registration::class_<NameComponent>("Name Component")
		.constructor<>()
		.property("name", &NameComponent::name);

	rttr::registration::class_<textComponent>("Text Component")
		.constructor<>()
		.property("text", &textComponent::text)
		.property("font", &textComponent::font)
		.property("color", &textComponent::color)
		.method("OnStart", &textComponent::OnStart)
		.method("OnUpdate", &textComponent::OnUpdate);

	rttr::registration::class_<MeshRenderComponent>("Mesh Render Component")
		.constructor<>()
		.property("color", &MeshRenderComponent::color)
		.property("shape", &MeshRenderComponent::shapeName)
		.method("OnStart", &MeshRenderComponent::OnStart)
		.method("OnUpdate", &MeshRenderComponent::OnUpdate);

	rttr::registration::class_<SpriteRenderComponent>("Sprite Render Component")
		.constructor<>()
		.property("color", &SpriteRenderComponent::color)
		.property("Texture", &SpriteRenderComponent::TextureString)
		.method("OnStart", &SpriteRenderComponent::OnStart)
		.method("OnUpdate", &SpriteRenderComponent::OnUpdate);

	rttr::registration::class_<lightComponent>("Light Component")
		.constructor<>()
		.property("Light Intensity", &lightComponent::lightIntensity)
		.method("OnStart", &lightComponent::OnStart)
		.method("OnUpdate", &lightComponent::OnUpdate);
		
	rttr::registration::class_<Script>("Script")
		.constructor<>()
		.method("OnStart", &Script::OnStart)
		.method("OnUpdate", &Script::OnUpdate);

;
}