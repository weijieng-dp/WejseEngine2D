//#include "EntityRegistry.h"
//#include "Registry.h"
//#include "iostream"
//
//class Scene {
//public:
//    Scene(const std::string& name) : name(name) {}
//
//    using Entity = EntityRegistry::Entity;
//
//    // Entity and Component management
//    void addEntity(Entity entity) {
//        registry.createEntity();
//    }
//
//    void removeEntity(Entity entity) {
//        entities.remove(entity);
//    }
//
//    template <typename T>
//    T* getComponent(Entity entity) {
//        return registry.getComponent<T>(entity);
//    }
//
//    // Scene-specific logic (e.g., camera, lighting)
//    void setCamera(const Camera& camera) {
//        this->camera = camera;
//    }
//
//    void setLighting(const Lighting& lighting) {
//        this->lighting = lighting;
//    }
//
//    // Scene management
//    void load(const std::string& filePath) {
//        // Load scene data from file
//    }
//
//    void save(const std::string& filePath) {
//        // Save current scene data to file
//    }
//
//    void update(float deltaTime) {
//        // Update all systems
//        renderSystem.update(deltaTime);
//        physicsSystem.update(deltaTime);
//        aiSystem.update(deltaTime);
//    }
//
//    void render() {
//        // Render all entities with render components
//        renderSystem.render();
//    }
//
//private:
//    std::string name;
//    std::vector<Entity> entities;   // List of entities in this scene
//    Registry registry;              // Registry managing the components
//    Camera camera;                  // Camera for this scene
//    Lighting lighting;              // Lighting for this scene
//
//    // Systems
//};
