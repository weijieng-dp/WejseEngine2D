#pragma once
#include <vector>
#include <string>
#include "unordered_set"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>
#include <iostream>
#define RTTR_DLL
#include "rttr/type.h"


#ifdef _WIN32
#ifdef REGISTRY_EXPORTS
#define REGISTRY_API __declspec(dllexport)  // When building the DLL
#else
#define REGISTRY_API __declspec(dllimport)  // When using the DLL
#endif
#else
#define REGISTRY_API  // Other platforms don't need this
#endif

// =====================================
// =      Start of EntityRegistry      =
// =====================================
class REGISTRY_API EntityRegistry
{
public:
	using Entity = uint32_t;

	Entity createEntityID();

    Entity createEntityID(int ID);

	void restartEntityCount();

	void destroyEntity(Entity entity);

	void addEntity(Entity entity, const std::string& name = "");

	std::vector<Entity> getAllEntity();

private:
	Entity nextId = 0;                 
	std::unordered_set<Entity> allEntities;  
};

// =====================================
// =     Start of ComponentStorage     =
// =====================================
class IComponentStorage {
public:
	virtual ~IComponentStorage() = default;
	virtual void remove(EntityRegistry::Entity entity) = 0; // Ensure this exists
	virtual rttr::instance getCompFromEntity(EntityRegistry::Entity entity) = 0; // Add this method
    virtual void clear() = 0;

};

// Generic component storage for a specific type
template <typename Component>
class ComponentStorage : public IComponentStorage {
public:
	void add(EntityRegistry::Entity entity, Component component) 
	{
		components[entity] = std::move(component);
	}

	void remove(EntityRegistry::Entity entity) 
	{
		components.erase(entity);
	}

	Component* get(EntityRegistry::Entity entity) 
	{
		if (components.find(entity) != components.end()) {
			return &components[entity];
		}
		return nullptr;
	}

	rttr::instance getCompFromEntity(EntityRegistry::Entity entity) override 
	{
		auto it = components.find(entity);
		if (it != components.end()) {
			return rttr::instance(it->second);
		}
		return rttr::instance();  // Return an empty instance when not found
	}

	const std::unordered_map<EntityRegistry::Entity, Component>& getComponents() const 
	{
		return components;
	}

	std::vector<EntityRegistry::Entity> getEntities() const
	{
		std::vector<EntityRegistry::Entity> result;
		result.reserve(components.size());
		for (const auto& pair : components) {
			result.push_back(pair.first);
		}
		return result;
	}
    
    void clear() override {
        components.clear();
    }

private:
	std::unordered_map<EntityRegistry::Entity, Component> components;
};

// =====================================
// =        Start of Registry          =
// =====================================

class REGISTRY_API Registry {
public:
    // Singleton Instance
    static Registry& instance();

    // Delete Copy Constructor & Assignment Operator (Singleton)
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;

    using Entity = EntityRegistry::Entity;

    // ===================================
    // ========== ENTITY SYSTEM ==========
    // ===================================

    void restartEntityCount();
    Entity createEntity();
    Entity createEntity(int ID);
    void destroyEntity(Entity entity);
    void destroyAllEntities();
    std::vector<Entity> getAllEntity();

    // ===================================
    // ===== COMPONENT MANAGEMENT ========
    // ===================================

    template <typename Component>
    void addComponent(Entity entity, Component component) {
        getStorage<Component>()->add(entity, std::move(component));
    }

    template <typename Component>
    Component* getComponent(Entity entity) {
        return getStorage<Component>()->get(entity);
    }

    template <typename Component>
    void removeComponent(Entity entity) {
        getStorage<Component>()->remove(entity);
    }

    void removeComponent(Entity ent, rttr::type type);
    std::vector<rttr::instance> getAllComponentsRTTR(Entity entity);
    rttr::instance getComponentsRTTR(Entity entity, rttr::type type);

    // ===================================
    // ======= QUERYING ENTITIES =========
    // ===================================

    template <typename Component>
    bool hasComponent(Entity entity) {
        auto storage = getStorage<Component>();
        return storage && storage->get(entity);
    }

    template <typename Component>
    std::vector<Entity> getEntitiesWithComponent() {
        std::vector<Entity> result;
        auto* storage = getStorage<Component>();
        if (!storage) return result;

        for (auto& pair : storage->getComponents()) {
            result.push_back(pair.first);
        }
        return result;
    }

    template <typename... Components>
    std::vector<Entity> getEntitiesWithComponents() {
        auto firstStorage = getStorage<std::tuple_element_t<0, std::tuple<Components...>>>();
        std::vector<Entity> result;

        for (Entity entity : firstStorage->getEntities()) {
            if ((hasComponent<Components>(entity) && ...)) {
                result.push_back(entity);
            }
        }
        return result;
    }
    void clearAllComponentStorages();
private:
    Registry() {} // Private constructor for singleton pattern

    EntityRegistry entityRegistry;
    std::map<rttr::type, std::unique_ptr<IComponentStorage>> componentStorages;

    // ===================================
    // ===== COMPONENT STORAGE ACCESS =====
    // ===================================

    template <typename Component>
    ComponentStorage<Component>* getStorage() {
        auto type = rttr::type::get<Component>();
        if (componentStorages.find(type) == componentStorages.end()) {
            componentStorages[type] = std::make_unique<ComponentStorage<Component>>();
        }
        return static_cast<ComponentStorage<Component>*>(componentStorages[type].get());
    }
};

// =====================================
// =   Start of Component Registry     =
// =====================================
class REGISTRY_API componentRegistry
{
public:
    static componentRegistry& instance();

	componentRegistry(const componentRegistry&) = delete;
	componentRegistry& operator=(const componentRegistry&) = delete;


	using ComponentCreator = std::function<void(EntityRegistry::Entity)>;


    std::vector<std::string> getComponentTypes() const;
    void createComponent(const std::string& name, EntityRegistry::Entity entity);


    template <typename Component>
    void registerComponent(const std::string& name, ComponentCreator creator) {
        componentCreators[name] = creator;
    }


    void unregisterComponent(const std::string& name);

private:
	componentRegistry() {};

	std::unordered_map<std::string, ComponentCreator> componentCreators;

};