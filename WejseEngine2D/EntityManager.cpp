//#include "EntityManager.h"
//#include "TransformComponent.h"
//#include "RenderComponent.h"
//#include "WejseRenderer.h"
//
//void EntityManager::CreateEntity()
//{
//	Entity ent = Entity();
//	ent.AddComponent(std::make_shared<TransformComponent>());
//	ent.AddComponent(std::make_shared<RenderComponent>());
//	Entities.push_back(ent);
//}
//
//
//
//void EntityManager::UpdateEntities()
//{
//	for (auto& entity : Entities) {
//		SetTransformMatrix(entity);
//		if (entity.hasComponent<RenderComponent>())
//		{
//			entity.getComponent<RenderComponent>()->RenderEntity();
//		}
//	}
//
//}
//
//
//void EntityManager::FreeEntity()
//{
//	for (auto& entity : Entities) {
//		// Update logic for entities (e.g., update components)
//
//
//		if (entity.hasComponent<RenderComponent>())
//		{
//			entity.getComponent<RenderComponent>()->CleanUpRenderer();
//		}
//	}
//}
//
//Entity EntityManager::FindEntity(int ID)
//{
//	for (auto& entity : Entities)
//	{
//		if (entity.getId() == ID)
//		{
//			return entity;
//		}
//	}
//}
//
//void EntityManager::RemoveEntity(int entID)
//{
//	auto it = Entities.begin();
//	while (it != Entities.end()) {
//		if (it->getId() == entID) {
//			it = Entities.erase(it);  // Erase and get the next valid iterator
//		}
//		else {
//			++it; // Only increment if no deletion happens
//		}
//	}
//
//}
//
//
