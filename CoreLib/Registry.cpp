#include "pch.h"
#include "Registry.h"
#include "../WejseEngine2D/ComponentFile.h"


// =====================================
// =      Start of EntityRegistry      =
// =====================================
EntityRegistry::Entity EntityRegistry::createEntityID()
{
	return ++nextId;
}

EntityRegistry::Entity EntityRegistry::createEntityID(int ID)
{
	return nextId = ID;
}

void EntityRegistry::restartEntityCount()
{
	nextId = 0;
}

void EntityRegistry::destroyEntity(Entity entity)
{
	allEntities.erase(entity);
}

void EntityRegistry::addEntity(Entity entity, const std::string& name)
{
	allEntities.insert(entity);
}

std::vector<EntityRegistry::Entity> EntityRegistry::getAllEntity()
{
	return std::vector<Entity>(allEntities.begin(), allEntities.end());;
}



// =====================================
// =        Start of Registry          =
// =====================================

std::vector<Registry::Entity> Registry::getAllEntity()
{
	return entityRegistry.getAllEntity();
}

void Registry::restartEntityCount()
{
	entityRegistry.restartEntityCount();
}

Registry::Entity Registry::createEntity()
{
	Entity entity = entityRegistry.createEntityID();
	entityRegistry.addEntity(entity);
	addComponent<NameComponent>(entity, {});
	addComponent<AnchorComponent>(entity, {});
	addComponent<ActiveComponent>(entity, {});

	return entity;
}

Registry::Entity Registry::createEntity(int ID)
{
	Entity entity = entityRegistry.createEntityID(ID);
	entityRegistry.addEntity(entity);
	addComponent<NameComponent>(entity, {});
	addComponent<AnchorComponent>(entity, {});
	addComponent<ActiveComponent>(entity, {});

	return entity;
}

void Registry::destroyAllEntities()
{
	for (Entity entity : entityRegistry.getAllEntity())
	{
		destroyEntity(entity);
	}
}

void Registry::clearAllComponentStorages()
{
	for (auto it = componentStorages.begin(); it != componentStorages.end(); )
	{
		// Store the current iterator for safe erasure
		it->second->clear();
		it = componentStorages.erase(it);  // Erase returns the next iterator
	}

}

void Registry::destroyEntity(Registry::Entity entity) {
	for (auto& pair : componentStorages) {
		auto& storage = pair.second;
		storage->remove(entity);
	}

	entityRegistry.destroyEntity(entity);
}

std::vector<rttr::instance> Registry::getAllComponentsRTTR(Registry::Entity entity) {
	std::vector<rttr::instance> componentsRTTR;
	for (auto& [typeIndex, storage] : componentStorages) 
	{
		rttr::instance component = storage->getCompFromEntity(entity);
		if (component.is_valid())
		{

			componentsRTTR.push_back(component);

		}
	}
	return componentsRTTR;
}

rttr::instance Registry::getComponentsRTTR(Registry::Entity entity, rttr::type type)
{
	auto it = componentStorages.find(type);
	if (it != componentStorages.end())
	{
		rttr::instance component = it->second->getCompFromEntity(entity);
		return component;
	}
	return rttr::instance();
}


void Registry::removeComponent(Registry::Entity ent, rttr::type type)
{

	auto it = componentStorages.find(type);
	
	if (it != componentStorages.end())
	{
		it->second->remove(ent);
	}
}

std::vector<std::string> componentRegistry::getComponentTypes() const
{
	std::vector<std::string> comp;
	for (auto& [type, func] : componentCreators)
	{
		comp.push_back(type);
	}

	return comp;
}

void componentRegistry::createComponent(const std::string& name, EntityRegistry::Entity entity) {
	if (componentCreators.find(name) != componentCreators.end()) {
		componentCreators[name](entity);
	}
}


componentRegistry& componentRegistry::instance() {
	static componentRegistry instance;
	return instance;
}

Registry& Registry::instance() {
	static Registry instance;
	return instance;
}

void componentRegistry::unregisterComponent(const std::string& name) {
	componentCreators.erase(name);
}