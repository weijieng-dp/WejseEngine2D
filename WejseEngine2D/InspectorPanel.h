#pragma once
#include "imgui.h"
#include "string"
#include "Registry.h"
#include "ComponentRegistry.h"

#include "WejseRenderer.h"
#include "RenderSystem.h"

#include "selectionComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

class InspectorPanel
{
public:

	void update()
	{
		auto entwithselectcomponet = registry.getEntitiesWithComponent<selectionComponent>();
		for (auto entity : entwithselectcomponet) {
			auto selectComponet = registry.getComponent<selectionComponent>(entity);
			if (selectComponet->selected && selectedComponentstring != "")
			{
				Componentregistry.createComponent(selectedComponentstring, entity);
				selectedComponentstring = "";
			}
		}
	}

	void render()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Inspector");
		auto entitywithtransformComponent = registry.getEntitiesWithComponents<TransformComponent, selectionComponent>();
		for (auto entity : entitywithtransformComponent)
		{
			auto selectioncomponent = registry.getComponent<selectionComponent>(entity);
			if (selectioncomponent->selected)
			{


				entityName = registry.getEntityName(entity);
				// Resize the entity name to handle up to 128 characters (if necessary)
				entityName.resize(128);

				if (ImGui::InputText("##EntityName", &entityName[0], entityName.size() + 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
					if (!entityName.empty()) {
						entityName.resize(strlen(entityName.c_str()));
						registry.setEntityName(entity, entityName);

						// Optionally print the updated name for debugging
						std::cout << "Updated Entity Name: " << registry.getEntityName(entity) << std::endl;

						// Optionally reset entityName back to the registry value if you want to clear input after updating
						entityName = registry.getEntityName(entity);  // Reset entity name after update
					}
				}


				auto tranformcomponent = registry.getComponent<TransformComponent>(entity);
				auto* rendercomponent = registry.getComponent<RenderComponent>(entity);


				ImGui::SetNextItemOpen(true);

				if (ImGui::TreeNode("Component"))
				{
					ImGui::SeparatorText("Transform Component");


					float TranslationValue[] = { tranformcomponent->translate.x, tranformcomponent->translate.y };
					float scaleValue[] = { tranformcomponent->scale.x,tranformcomponent->scale.y };

					ImGui::DragFloat2("Position: ", TranslationValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
					ImGui::DragFloat("Rotation: ", &tranformcomponent->rotation, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
					ImGui::DragFloat2("Scale: ", scaleValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					tranformcomponent->translate = glm::vec3(TranslationValue[0], TranslationValue[1], 0);
					tranformcomponent->scale = glm::vec3(scaleValue[0], scaleValue[1], 1);


					if (rendercomponent)
					{
						ImGui::Dummy(ImVec2(0.0f, 10.0f));

						ImGui::SeparatorText("Render Component");

						// Ensure the string has enough space for input
						rendercomponent->TextureString.resize(128); // Adjust size as needed

						// Text input field
						if (ImGui::InputText("Input Text", &rendercomponent->TextureString[0], rendercomponent->TextureString.size() + 1)) {
							// Optional: Resize back to the actual length
							rendercomponent->TextureString.resize(strlen(rendercomponent->TextureString.c_str()));
						}

						// Show the current value of the input
						if (ImGui::Button("Change Texture"))
						{
							updateTexture(rendercomponent->TextureString, entity);
						}
						if (ImGui::Button("Delete Component")) {
							registry.removeComponent<RenderComponent>(entity);
						}
					}

					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					if (ImGui::Button("Add Component")) {
						ImGui::OpenPopup("ComponentSelectorPopup");
					}

					if (ImGui::BeginPopup("ComponentSelectorPopup")) {
						ImGui::Text("Components");
						ImGui::Separator();

						for (const auto& componentName : Componentregistry.getComponentTypes()) {
							if (ImGui::Selectable(componentName.c_str())) {
								selectedComponentstring = componentName; // Update the selected component
							}
						}


						ImGui::EndPopup();
					}


					ImGui::TreePop();
				}

				if (io.KeysDown[ImGuiKey_Delete])
				{
					registry.destroyEntity(entity);
				}
			}
		}
	}
private:
	Registry& registry = Registry::instance();
	componentRegistry& Componentregistry = componentRegistry::instance();

	std::string entityName;
	std::string selectedComponentstring;
};