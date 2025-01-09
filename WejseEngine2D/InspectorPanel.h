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
#include "CameraComponent.h"


#include <rttr/registration.h>

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
		rttr::type transformType = rttr::type::get<TransformComponent>();

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


				ImGui::SetNextItemOpen(true);

				

				if (ImGui::TreeNode("Component"))
				{

					
						RenderDynamicUI<TransformComponent>(entity);
						RenderDynamicUI<SpriteRenderComponent>(entity);
						RenderDynamicUI<MeshRenderComponent>(entity);
						RenderDynamicUI<CameraComponent>(entity);

		

								



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
		ImGui::End();

	}

	// Render function to display properties dynamically
	template<typename t>
	void RenderDynamicUI(EntityRegistry::Entity ent) {

		auto comp = registry.getComponent<t>(ent);
		// Get the type of the object dynamically

		if (comp)
		{
			
			rttr::type obj_type = rttr::type::get(comp);
			ImGui::SeparatorText(obj_type.get_name().to_string().c_str());
			
			// Iterate through the object's properties
			for (auto& prop : obj_type.get_properties()) 
			{
				std::string prop_name = prop.get_name().to_string();
				auto value = prop.get_value(comp);

				if (value.is_type<int>()) {
					int int_value = value.get_value<int>();
					if (ImGui::InputInt(prop_name.c_str(), &int_value)) {
						prop.set_value(comp, int_value);  // Update the object property
					}
				}
				else if (value.is_type<std::string>() && prop_name == "shape")
				{


					static std::string items[] = { "square", "triangle", "circle","line" };
					static std::string currentItem = value.get_value<std::string>();

					if (ImGui::BeginCombo("mesh", currentItem.c_str())) {
						for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
							bool isSelected = (currentItem == items[i]);
							if (ImGui::Selectable(items[i].c_str(), isSelected)) {
								std::cout << prop.set_value(comp, items[i]);
								currentItem = items[i];
							}
						}
						ImGui::EndCombo();
					}
				}
				else if (value.is_type<std::string>() && prop_name == "Texture")
				{
					std::string string_value = value.get_value<std::string>();
					// Ensure that the string has enough space for input
					if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
						// Update the object property with the new string length (truncated to fit)
						string_value.resize(strlen(string_value.c_str()));
						prop.set_value(comp, string_value);  // Update the object property
					}

					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
							path = static_cast<const char*>(payload->Data);
							if (path)
							{
								updateTexture(path, ent);
								string_value = path;
							}
						}
						ImGui::EndDragDropTarget();
					}

				}
				else if (value.is_type<std::string>()) {
					std::string string_value = value.get_value<std::string>();
					// Ensure that the string has enough space for input
					if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
						// Update the object property with the new string length (truncated to fit)
						string_value.resize(strlen(string_value.c_str()));
						prop.set_value(comp, string_value);  // Update the object property
					}
				}
				else if (value.is_type<glm::vec3>() && prop_name == "color") {
					glm::vec3 vec_value = value.get_value<glm::vec3>();
					if (ImGui::ColorEdit3(prop_name.c_str(), &vec_value[0], 0.1f)) {
						prop.set_value(comp, vec_value);  // Update the object property
					}
				}
				else if (value.is_type<glm::vec3>()) {
					glm::vec3 vec_value = value.get_value<glm::vec3>();
					if (ImGui::DragFloat3(prop_name.c_str(), &vec_value[0], 0.1f)) {
						prop.set_value(comp, vec_value);  // Update the object property
					}
				}
				else if (value.is_type<float>()) {
					float float_value = value.get_value<float>();
					if (ImGui::DragFloat(prop_name.c_str(), &float_value, 0.1f)) {
						prop.set_value(comp, float_value);  // Update the object property
					}
				}

			}
			if (obj_type.get_name().to_string() != "Transform Component*")
			{
				if (ImGui::Button(("Delete Component ##" + obj_type.get_name().to_string()).c_str())) {
					std::cout << "Delete button clicked for component: " << obj_type.get_name().to_string() << std::endl;
					registry.removeComponent<t>(ent);
				}

			}
			ImGui::Dummy(ImVec2(0, 10));
		}
	}


private:
	Registry& registry = Registry::instance();
	componentRegistry& Componentregistry = componentRegistry::instance();

	std::string entityName;
	std::string selectedComponentstring;
	const char* path;

};