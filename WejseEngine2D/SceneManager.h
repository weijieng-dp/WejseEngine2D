#pragma once
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Registry.h"

#include "selectionComponent.h"
#include "TransformComponent.h"
#include "SpriteRenderComponent.h"
#include "meshRenderComponent.h"
#include "CameraComponent.h"
#include "ComponentRegistry.h"

#include "SpriteRenderSystem.h"

#include "rttr/type.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace rapidjson;

class SceneManager
{
public:
	SceneManager() : allocator(doc.GetAllocator()) {}

	void createScene()
	{

	}

	void SaveScene(const char* filepath)
	{
		doc.SetObject();



		doc.AddMember("entities", Serialize(), allocator);

		std::ofstream outFile(filepath);
		if (!outFile)
		{
			std::cerr << "Error: Unable to open file for saving: " << filepath << std::endl;
			return;
		}

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);

		outFile << buffer.GetString();
		outFile.close();
	}

	void LoadScene(const char* filepath)
	{
		std::ifstream inFile(filepath);
		if (!inFile)
		{
			std::cerr << "Error: Unable to open file for loading: " << filepath << std::endl;
			return;
		}

		std::string jsonContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		Document doc2;
		doc2.Parse(jsonContent.c_str());

		if (!doc2.IsObject() || !doc2.HasMember("entities") || !doc2["entities"].IsArray())
		{
			std::cerr << "Error: Invalid scene format!" << std::endl;
			return;
		}



		registry.DestroyAllEntities();

		Deserialize(doc2["entities"]);
	}




	template <typename T>
	void DynamicSerializer( rapidjson::Value& entJson, EntityRegistry::Entity ent) {
		auto comp = registry.getComponent<T>(ent); // Assumes `registry` is accessible

		if (comp) 
		{
			Value compJson(kObjectType);


			rttr::type obj_type = rttr::type::get(comp); // Get type information for the component


			// Iterate through the object's properties
			for (auto& prop : obj_type.get_properties()) 
			{
				const std::string prop_name = prop.get_name().to_string();
				auto value = prop.get_value(comp); // Use *comp since `comp` is a pointer

				if (value.is_type<int>()) 
				{
				
					int actualValue = value.get_value<int>();

					compJson.AddMember(
						rapidjson::Value(prop_name.c_str(), allocator),
						Value(actualValue),  // Directly using the int value
						allocator
					);
				}
				else if (value.is_type<glm::vec3>()) {
					const glm::vec3 vec = value.get_value<glm::vec3>();

					// Create a JSON array
					rapidjson::Value jsonArray(rapidjson::kArrayType);
					jsonArray.PushBack(vec[0], allocator); // Add x-component
					jsonArray.PushBack(vec[1], allocator); // Add y-component
					jsonArray.PushBack(vec[2], allocator); // Add z-component

					// Add the array to the JSON object
					compJson.AddMember(
						rapidjson::Value(prop_name.c_str(), allocator),
						jsonArray,
						allocator
					);
				}
				else if (value.is_type<std::string>()) 
				{
					compJson.AddMember(
						rapidjson::Value(prop_name.c_str(), allocator),
						rapidjson::Value(value.get_value<std::string>().c_str(), allocator),
						allocator
					);
				}
				else if (value.is_type<float>()) 
				{
					float  actualValue = value.get_value<float>();
					compJson.AddMember(
						rapidjson::Value(prop_name.c_str(), allocator),
						Value(actualValue),  // Directly using the int value
						allocator
					);
				}
			}

			// Add the component JSON object to the main array
			entJson.AddMember(
				rapidjson::Value(obj_type.get_name().to_string().c_str(), allocator),
				compJson,
				allocator
			);
		}
	}


	Value Serialize()
	{
		Value entitiesArr(kArrayType);


		auto entities = registry.getAllEntity();
		for (auto entity : entities)
		{
			Value entityJson(kObjectType);
			entityJson.AddMember("name", Value(registry.getEntityName(entity).c_str(), allocator), allocator);

			DynamicSerializer<TransformComponent>(entityJson, entity);
			DynamicSerializer<SpriteRenderComponent>(entityJson, entity);
			DynamicSerializer<MeshRenderComponent>(entityJson, entity);
			DynamicSerializer<CameraComponent>(entityJson, entity);
		

			entitiesArr.PushBack(entityJson, allocator);
		}
		return entitiesArr;

	}
	void Deserialize(const Value& entities)
	{
		for (const auto& entityJson : entities.GetArray())
		{
			if (!entityJson.IsObject() || !entityJson.HasMember("name"))
			{
				std::cerr << "Error: Invalid entity format or missing 'name' in entity." << std::endl;
				continue;
			}

			auto ent = registry.createEntity(entityJson["name"].GetString());

			registry.addComponent<selectionComponent>(ent, {});

			DynamicDeserializer<TransformComponent>(entityJson, ent);
			DynamicDeserializer<MeshRenderComponent>(entityJson, ent);
			DynamicDeserializer<SpriteRenderComponent>(entityJson, ent);
			DynamicDeserializer<CameraComponent>(entityJson, ent);
			DynamicDeserializer<LayerComponent>(entityJson, ent);

		}
	}

	template <typename T>
	void DynamicDeserializer(const rapidjson::Value& entJson, EntityRegistry::Entity ent) {

		if (typeid(T) == typeid( MeshRenderComponent)) {
			if (entJson.HasMember("Mesh Render Component*") && entJson["Mesh Render Component*"].IsObject()) {
				const auto& meshrendererJson = entJson["Mesh Render Component*"];

				// Validate "color"
				glm::vec3 color(1.0f, 1.0f, 1.0f); // Default value
				if (meshrendererJson.HasMember("color") && meshrendererJson["color"].IsArray() && meshrendererJson["color"].Size() == 3) {
					const auto& colorArray = meshrendererJson["color"];
					if (colorArray[0].IsFloat() && colorArray[1].IsFloat() && colorArray[2].IsFloat()) {
						color = glm::vec3(
							colorArray[0].GetFloat(),
							colorArray[1].GetFloat(),
							colorArray[2].GetFloat()
						);
					}
				}

				// Validate "shape"
				std::string shape = meshrendererJson.HasMember("shape") && meshrendererJson["shape"].IsString()
					? meshrendererJson["shape"].GetString()
					: "square";

				// Create component and add to registry
				MeshRenderComponent meshrenderComp("shader/shader.vs", "shader/shader.fs", color, shape);
				registry.addComponent<MeshRenderComponent>(ent, std::move(meshrenderComp));
			}
			return;
		}

		if (typeid(T) == typeid(SpriteRenderComponent)) {
			if (entJson.HasMember("Sprite Render Component*") && entJson["Sprite Render Component*"].IsObject()) {
				const auto& SpriterendererJson = entJson["Sprite Render Component*"];

				// Validate "color"
				glm::vec3 color(1.0f, 1.0f, 1.0f); // Default value
				if (SpriterendererJson.HasMember("color") && SpriterendererJson["color"].IsArray() && SpriterendererJson["color"].Size() == 3) {
					const auto& colorArray = SpriterendererJson["color"];
					if (colorArray[0].IsFloat() && colorArray[1].IsFloat() && colorArray[2].IsFloat()) {
						color = glm::vec3(
							colorArray[0].GetFloat(),
							colorArray[1].GetFloat(),
							colorArray[2].GetFloat()
						);
					}
				}

				// Validate "texture"
				std::string texture = SpriterendererJson.HasMember("Texture") && SpriterendererJson["Texture"].IsString()
					? SpriterendererJson["Texture"].GetString()
					: "Assets/PlanetTexture.png";

				SpriteRenderComponent spriterenderComp(
					"shader/shader.vs",
					"shader/shader.fs",
					texture,
					color
				);
				
				registry.addComponent<SpriteRenderComponent>(ent, std::move(spriterenderComp));
				//updateTexture(texture, ent);
			}
			return;
		}

		auto comp = registry.getComponent<T>(ent);
		rttr::type obj_type = rttr::type::get(comp);


		if (!entJson.HasMember(obj_type.get_name().to_string().c_str()) || !entJson[obj_type.get_name().to_string().c_str()].IsObject())
		{
			return;
		}

		// Check if the component exists or create it
		if (!comp) {
			 registry.addComponent<T>(ent, {}); // Assumes `addComponent` exists
			 comp = registry.getComponent<T>(ent);

		}
		// Get type information for the component


		auto properties = obj_type.get_properties();

		if (properties.empty()) {
			// Skip deserialization for components without properties
			return;
		}

		// Iterate through the JSON object
		
		for (auto& member : entJson[obj_type.get_name().to_string().c_str()].GetObject()) {

			const std::string prop_name = member.name.GetString();
			const rapidjson::Value& prop_value = member.value;

			// Find the property by name in the component type
			rttr::property prop = obj_type.get_property(prop_name);
			if (!prop.is_valid()) {
				std::cerr << "Unknown property: " << prop_name << std::endl;
				continue;
			}

			// Set the property value based on its type
			if (prop.get_type() == rttr::type::get<int>()) {
				if (prop_value.IsInt()) {
					prop.set_value(comp, prop_value.GetInt());
				}
			}
			else if (prop.get_type() == rttr::type::get<float>()) {
				if (prop_value.IsFloat()) {
					prop.set_value(comp, prop_value.GetFloat());
				}
			}
			else if (prop.get_type() == rttr::type::get<std::string>()) {
				if (prop_value.IsString()) {
					prop.set_value(comp, std::string(prop_value.GetString()));
				}
			}
			else if (prop.get_type() == rttr::type::get<glm::vec3>()) {
				if (prop_value.IsArray() && prop_value.Size() == 3) {
					glm::vec3 vec(
						prop_value[0].GetFloat(),
						prop_value[1].GetFloat(),
						prop_value[2].GetFloat()
					);
					prop.set_value(comp, vec);
				}
			}
			else {
				std::cerr << "Unsupported property type: " << prop_name << std::endl;
			}
		}
	}



private:
	Registry& registry = Registry::instance();
	componentRegistry& ComponentRegistry = componentRegistry::instance();

	Document doc;
	Document::AllocatorType& allocator;

};

