#pragma once
#include "EntityRegistry.h"
#include "component.h"

class Registry {
public:

    static Registry& instance() {
        static Registry instance;  // Created lazily
        return instance;
    }

    // Prevent copying and assignment
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;
    


    using Entity = EntityRegistry::Entity;

    std::vector<Entity> getAllEntity()
    {
        return entityRegistry.getAllEntity();
    }

    void setEntityName(Entity ent, std::string name)
    {
        entityRegistry.setEntityName(ent, name);
    }

    std::string getEntityName(Entity ent)
    {
        return entityRegistry.getEntityName(ent);
    }

    // Create a new entity
    Entity createEntity(std::string name = "") {
        Entity entity = entityRegistry.createEntity();
        entityRegistry.addEntity(entity,name);
        return entity;
    }

    void DestroyAllEntities()
    {
        for (Entity entity : entityRegistry.getAllEntity())
        {
            destroyEntity(entity);
        }
    }

    // Destroy an entity and its components
    void destroyEntity(Entity entity) {
        for (auto& pair : componentStorages) {
            auto& type = pair.first;
            auto& storage = pair.second;
            storage->remove(entity);
        }

        entityRegistry.destroyEntity(entity);
    }

    // Add a component to an entity
    template <typename Component>
    void addComponent(Entity entity, Component component) {
        getStorage<Component>()->add(entity, std::move(component));
    }

    // Get a component from an entity
    template <typename Component>
    Component* getComponent(Entity entity) {
        return getStorage<Component>()->get(entity);
    }



    // Remove a component from an entity
    template <typename Component>
    void removeComponent(Entity entity) {
        getStorage<Component>()->remove(entity);
    }



    template <typename Component>
    std::vector<Entity> getEntitiesWithComponent() {
        std::vector<Entity> result;
        auto* storage = getStorage<Component>();
        if (!storage) return result;

        for (auto& pair : storage->getComponents()) {
            auto& entity = pair.first;
            auto& component = pair.second;
            result.push_back(entity);
        }
        return result;
    }



    template <typename Component>
    bool hasComponent(Entity entity)
    {
        auto storage = getStorage<Component>();
        return storage && storage->get(entity);
    }



    template <typename... Components>
    std::vector<Entity> getEntitiesWithComponents() {
        // Start with entities from the storage of the smallest component set
        auto firstStorage = getStorage<std::tuple_element_t<0, std::tuple<Components...>>>();
        std::vector<Entity> result;

        for (Entity entity : firstStorage->getEntities()) {
            // Check if the entity has all other components
            if ((hasComponent<Components>(entity) && ...)) {
                result.push_back(entity);
            }
        }

        return result;
    }


private:

    Registry() {}

    EntityRegistry entityRegistry;

    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> componentStorages;


    // Get or create storage for a specific component type
    template <typename Component>
    ComponentStorage<Component>* getStorage() {
        auto type = std::type_index(typeid(Component));
        if (componentStorages.find(type) == componentStorages.end()) {
            componentStorages[type] = std::make_unique<ComponentStorage<Component>>();
        }
        return static_cast<ComponentStorage<Component>*>(componentStorages[type].get());
    }
};
