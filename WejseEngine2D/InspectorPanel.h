#pragma once
#include "imgui.h"
#include "string"
#include "Registry.h"
#include "ComponentRegistry.h"

#include "WejseRenderer.h"
#include "SpriteRenderSystem.h"

#include "selectionComponent.h"
#include "TransformComponent.h"
#include "SpriteRenderComponent.h"
#include "meshRenderComponent.h"

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
				auto* spriteRenderComponent = registry.getComponent<SpriteRenderComponent>(entity);
				auto* meshRenderComponent = registry.getComponent<MeshRenderComponent>(entity);

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


					if (spriteRenderComponent)
					{
						ImGui::Dummy(ImVec2(0.0f, 10.0f));

						ImGui::SeparatorText("Sprite Render Component");

						float uColor[3] = { spriteRenderComponent->color[0],spriteRenderComponent->color[1] ,spriteRenderComponent->color[2] };

						if(ImGui::ColorEdit3("Vertex Color", uColor)) {
							// Color has changed; you can use it to update the OpenGL buffer or shaders
							spriteRenderComponent->color[0] = uColor[0];
							spriteRenderComponent->color[1] = uColor[1];
							spriteRenderComponent->color[2] = uColor[2];
						}



						// Ensure the string has enough space for input
						spriteRenderComponent->TextureString.resize(128); // Adjust size as needed


						// Text input field
						if (ImGui::InputText("Input Text", &spriteRenderComponent->TextureString[0], spriteRenderComponent->TextureString.size() + 1)) {
							// Optional: Resize back to the actual length
							spriteRenderComponent->TextureString.resize(strlen(spriteRenderComponent->TextureString.c_str()));
						}


						ImVec2 size = ImGui::GetItemRectSize();
						ImGui::InvisibleButton("DropZone", ImVec2(size.x, size.y));
						if (ImGui::BeginDragDropTarget()) {
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
								path = static_cast<const char*>(payload->Data);
								if (path)
								{
									updateTexture(path, entity);
									spriteRenderComponent->TextureString = path;
								}
							}
							ImGui::EndDragDropTarget();
						}


						// Show the current value of the input
						if (ImGui::Button("Change Texture"))
						{
							updateTexture(spriteRenderComponent->TextureString, entity);
						}
						if (ImGui::Button("Delete Component")) {
							registry.removeComponent<SpriteRenderComponent>(entity);
						}
					}

					if (meshRenderComponent)
					{

						ImGui::Dummy(ImVec2(0.0f, 10.0f));

						ImGui::SeparatorText("Mesh Render Component");

						static const char* items[] = { "square", "triangle", "circle","line"};
						static std::string currentItem = meshRenderComponent->shapeName;

						if (ImGui::BeginCombo("mesh", currentItem.c_str())) {
							for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
								bool isSelected = (currentItem == items[i]);
								if (ImGui::Selectable(items[i], isSelected)) {
									meshRenderComponent->shapeName = items[i];
									currentItem = items[i];
								}
							}
							ImGui::EndCombo();
						}

						float uColor[3] = { meshRenderComponent->color[0],meshRenderComponent->color[1] ,meshRenderComponent->color[2] };

						if (ImGui::ColorEdit3("Vertex Color", uColor)) {
							// Color has changed; you can use it to update the OpenGL buffer or shaders
							meshRenderComponent->color[0] = uColor[0];
							meshRenderComponent->color[1] = uColor[1];
							meshRenderComponent->color[2] = uColor[2];
						}

						if (ImGui::Button("Delete Component")) {
							registry.removeComponent<MeshRenderComponent>(entity);
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
	const char* path;
};