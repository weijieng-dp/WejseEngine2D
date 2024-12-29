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
#include "RenderComponent.h"
#include "ComponentRegistry.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace rapidjson;

class SceneManager
{
public:
	SceneManager() : allocator(doc.GetAllocator()) {}

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


		Deserialize(doc2["entities"]);
	}

	Value Serialize()
	{
		Value entitiesArr(kArrayType);

		auto entities = registry.getAllEntity();
		for (auto entity : entities)
		{
			Value entityJson(kObjectType);

			entityJson.AddMember("name", Value(registry.getEntityName(entity).c_str(), allocator), allocator);


			auto* transform = registry.getComponent<TransformComponent>(entity);

			if (transform)
			{
				Value transformJson(kObjectType);
				transformJson.AddMember("posX", transform->translate.x, allocator);
				transformJson.AddMember("posY", transform->translate.y, allocator);

				transformJson.AddMember("rotation", transform->rotation, allocator);

				transformJson.AddMember("scaleX", transform->scale.x, allocator);
				transformJson.AddMember("scaleY", transform->scale.y, allocator);

				entityJson.AddMember("Transform", transformJson, allocator);
			}

			auto* renderer = registry.getComponent<RenderComponent>(entity);

			if (renderer)
			{
				Value rendererJson(kObjectType);
				rendererJson.AddMember("shaderVertexPath", "shader/shader.vs", allocator);
				rendererJson.AddMember("shaderfragmentPath", "shader/shader.fs", allocator);
				rendererJson.AddMember("texturePath", Value((renderer->TextureString).c_str(), allocator), allocator);


				entityJson.AddMember("Renderer", rendererJson, allocator);
			}

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

			if (entityJson.HasMember("Transform") && entityJson["Transform"].IsObject())
			{
				const auto& transformJson = entityJson["Transform"];
				TransformComponent transform;
				transform.translate.x = transformJson["posX"].GetFloat();
				transform.translate.y = transformJson["posY"].GetFloat();
				transform.rotation = transformJson["rotation"].GetFloat();
				transform.scale.x = transformJson["scaleX"].GetFloat();
				transform.scale.y = transformJson["scaleY"].GetFloat();

				registry.addComponent<TransformComponent>(ent, transform);
			}
			else
			{
				std::cerr << "Error: Missing 'Transform' in entity: " << entityJson["name"].GetString() << std::endl;
			}

			// Deserialize RenderComponent
			if (entityJson.HasMember("Renderer") && entityJson["Renderer"].IsObject())
			{
				const auto& rendererJson = entityJson["Renderer"];

				registry.addComponent<RenderComponent>(ent, std::move(RenderComponent(
					rendererJson["shaderVertexPath"].GetString(),
					rendererJson["shaderfragmentPath"].GetString(),
					rendererJson["texturePath"].GetString()
				)));


			}
		}
	}
private:
	Registry& registry = Registry::instance();
	componentRegistry& ComponentRegistry = componentRegistry::instance();

	Document doc;
	Document::AllocatorType& allocator;

};

