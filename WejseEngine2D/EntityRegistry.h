#pragma once
#include <vector>
#include <queue>
#include <string>
#include "unordered_set"
#include <unordered_map>

class EntityRegistry {
public:
    using Entity = uint32_t;

    // Create a new entity
    Entity createEntity() {
        if (!freeIds.empty()) {
            Entity id = freeIds.front();
            freeIds.pop();
            return id;
        }
        return nextId++;
    }

    // Destroy an entity
    void destroyEntity(Entity entity) {
        allEntities.erase(entity);
        entityNames.erase(entity);
        freeIds.push(entity);
    }

    void setEntityName(Entity entity, const std::string& name) {
        for (const auto& pair : entityNames) {
            if (pair.second == name) {
                return; // Avoid duplicate names
            }
        }
        entityNames[entity] = name;
    }

    std::string getEntityName(Entity entity) const {
        auto it = entityNames.find(entity);
        return (it != entityNames.end()) ? it->second : "Unnamed";
    }

    void addEntity(Entity entity, const std::string& name = "")
    {
        allEntities.insert(entity);
        entityNames[entity] = name.empty() ? "Entity_" + std::to_string(entity) : name;

    }

    std::vector<Entity> getAllEntity()
    {
        return std::vector<Entity>(allEntities.begin(), allEntities.end());;
    }
private:
    Entity nextId = 0;                 // Next available entity ID
    std::queue<Entity> freeIds;       // Recycled entity IDs
    std::unordered_map<Entity, std::string> entityNames;
    
    std::unordered_set<Entity> allEntities;  // Stores all created entities
};


