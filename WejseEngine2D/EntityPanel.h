#pragma once
#include "selectionComponent.h"
#include "imgui.h"
#include "string"
#include "Registry.h"
#include "TransformComponent.h"
#include "ComponentRegistry.h"

class EntityPanel
{
public:
	
	void render()
	{
		ImGui::Begin("Entities");


		auto entitywithselectComponent = registry.getEntitiesWithComponent<selectionComponent>();
		for (auto entity : entitywithselectComponent)
		{
			//EncodeEntityID(manager.Entities[n]);


			auto selectComponent = registry.getComponent<selectionComponent>(entity);

			std::string entities = registry.getEntityName(entity);


			if (ImGui::Selectable(entities.c_str(), selectComponent->selected))
			{

				if (!ImGui::GetIO().KeyCtrl)  // Clear selection if CTRL is not held
				{
					// Deselect all entities when CTRL is not held
					for (auto entity2 : entitywithselectComponent)
					{
						auto selectedComponent = registry.getComponent<selectionComponent>(entity2);
						selectedComponent->selected = false;
					}
				}

				// Toggle the selection state for the current entity
				selectComponent->selected = !selectComponent->selected;


			}


		}

		if (ImGui::Button("Add Entities"))
		{
			auto entities = registry.createEntity();
			registry.addComponent<TransformComponent>(entities, {});
			Componentregistry.createComponent("Selection Component", entities);
		}



		ImGui::End();
	}
private:
	Registry& registry = Registry::instance();
	componentRegistry& Componentregistry = componentRegistry::instance();
};