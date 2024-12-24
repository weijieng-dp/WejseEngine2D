#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <typeindex>
#include <vector>
#include "EntityRegistry.h"

class componentRegistry
{
public:
    static componentRegistry& instance() {
        static componentRegistry instance;  // Created lazily
        return instance;
    }

    // Prevent copying and assignment
    componentRegistry(const componentRegistry&) = delete;
    componentRegistry& operator=(const componentRegistry&) = delete;


    using ComponentCreator = std::function<void(EntityRegistry::Entity)>;

    // Register a component type
    template <typename Component>
    void registerComponent(const std::string& name, ComponentCreator creator) {
        componentCreators[name] = creator;
        componentTypes.push_back(name);
    }

    // Get a list of all registered component types
    std::vector<std::string> getComponentTypes() const {
        return componentTypes;
    }

    // Create a component for an entity
    void createComponent(const std::string& name, EntityRegistry::Entity entity) {
        if (componentCreators.find(name) != componentCreators.end()) {
            componentCreators[name](entity);
        }
    }

private:
    componentRegistry() {};

    std::unordered_map<std::string, ComponentCreator> componentCreators;
    std::vector<std::string> componentTypes;

};
