#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "EntityRegistry.h"

// Base class for all component storage
class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    virtual void remove(EntityRegistry::Entity entity) = 0; // Ensure this exists
};


// Generic component storage for a specific type
template <typename Component>
class ComponentStorage : public IComponentStorage {
public:
    void add(EntityRegistry::Entity entity, Component component) {
        components[entity] = std::move(component);
    }

    void remove(EntityRegistry::Entity entity) {
        components.erase(entity);
    }

    Component* get(EntityRegistry::Entity entity) {
        if (components.find(entity) != components.end()) {
            return &components[entity];
        }
        return nullptr;
    }

    const std::unordered_map<EntityRegistry::Entity, Component>& getComponents() const {
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

private:
    std::unordered_map<EntityRegistry::Entity, Component> components;
};
