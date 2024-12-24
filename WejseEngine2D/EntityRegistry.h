#pragma once
#include <vector>
#include <queue>
#include "unordered_set"

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
        freeIds.push(entity);
    }


    void addEntity(Entity entity)
    {
        allEntities.insert(entity);
    }

    std::vector<Entity> getAllEntity()
    {
        return std::vector<Entity>(allEntities.begin(), allEntities.end());;
    }
private:
    Entity nextId = 0;                 // Next available entity ID
    std::queue<Entity> freeIds;       // Recycled entity IDs
    std::unordered_set<Entity> allEntities;  // Stores all created entities
};


