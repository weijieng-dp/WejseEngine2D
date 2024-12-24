//#pragma once
//#include <vector>
//#include "Entity.h"
//#include "component.h"
//#include <algorithm>
//
//class EntityManager
//{
//private:
//	 // Private constructor to prevent external instantiation
//    EntityManager() {}
//
//    // Delete the copy constructor and assignment operator to ensure Singleton behavior
//    EntityManager(const EntityManager&) = delete;
//    EntityManager& operator=(const EntityManager&) = delete;
//public:
//	static EntityManager& GetInstance() {
//		static EntityManager instance; // Guaranteed to be destroyed, instantiated on first use
//		return instance;
//	}
//	void CreateEntity();
//	std::vector<Entity> Entities;
//	void RemoveEntity(int entID);
//	void UpdateEntities();
//	void FreeEntity();
//	Entity FindEntity(int ID);
//
//};
//
