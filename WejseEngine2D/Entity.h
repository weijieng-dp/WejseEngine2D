//#pragma once
//#include <unordered_map>;
//#include <iostream>
//#include <string>
//#include "component.h"
//
//class Entity
//{
//private:
//	std::unordered_map<std::type_index, std::shared_ptr<component>> components;
//	int id;
//	int static nextid;
//public:
//	Entity();
//    bool selected = false;
//    std::string name;
//
//	int getId() const { return id; }
//
//
//
//    template <typename T>
//    void AddComponent(std::shared_ptr<T> component) {
//        components[std::type_index(typeid(T))] = component;
//    }
//
//    template <typename T>
//    void RemoveComponent(std::shared_ptr<T> component) {
//        auto it = components.find(std::type_index(typeid(T)));
//        if (it != components.end() && it->second == component) {
//            components.erase(it);
//        }
//    }
//
//    template <typename T>
//    std::shared_ptr<T> getComponent() {
//        auto it = components.find(std::type_index(typeid(T)));
//        if (it != components.end()) {
//            return std::static_pointer_cast<T>(it->second);
//        }
//        return nullptr;
//    }
//
//    template <typename T>
//    bool hasComponent() const {
//        return components.find(std::type_index(typeid(T))) != components.end();
//    }
//
//};
//
