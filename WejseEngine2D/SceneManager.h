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
#include "ComponentRegistry.h"

#include "SpriteRenderSystem.h"


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

			auto* renderer = registry.getComponent<SpriteRenderComponent>(entity);

			if (renderer)
			{
				Value rendererJson(kObjectType);
				rendererJson.AddMember("shaderVertexPath", "shader/shader.vs", allocator);
				rendererJson.AddMember("shaderfragmentPath", "shader/shader.fs", allocator);
				rendererJson.AddMember("texturePath", Value((renderer->TextureString).c_str(), allocator), allocator);
				rendererJson.AddMember("colorX",renderer->color[0],allocator);
				rendererJson.AddMember("colorY", renderer->color[1],allocator);
				rendererJson.AddMember("colorZ", renderer->color[2],allocator);


				entityJson.AddMember("Renderer", rendererJson, allocator);
			}

			auto* meshRenderer = registry.getComponent<MeshRenderComponent>(entity);

			if (meshRenderer)
			{
				Value meshrendererJson(kObjectType);

				meshrendererJson.AddMember("shapeName", Value((meshRenderer->shapeName).c_str(), allocator), allocator);
				meshrendererJson.AddMember("colorX", meshRenderer->color[0], allocator);
				meshrendererJson.AddMember("colorY", meshRenderer->color[1], allocator);
				meshrendererJson.AddMember("colorZ", meshRenderer->color[2], allocator);

				entityJson.AddMember("meshRenderer", meshrendererJson, allocator);

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

			// Deserialize SpriteRenderComponent
			if (entityJson.HasMember("meshRenderer") && entityJson["meshRenderer"].IsObject())
			{
				const auto& meshrendererJson = entityJson["meshRenderer"];
				MeshRenderComponent meshrenderComp = MeshRenderComponent(
					"shader/shader.vs",
					"shader/shader.fs",
					glm::vec3(meshrendererJson["colorX"].GetFloat(), meshrendererJson["colorY"].GetFloat(), meshrendererJson["colorZ"].GetFloat()),
					meshrendererJson["shapeName"].GetString());

				registry.addComponent<MeshRenderComponent>(ent, std::move(meshrenderComp));

			}

			if (entityJson.HasMember("Renderer") && entityJson["Renderer"].IsObject())
			{
				const auto& rendererJson = entityJson["Renderer"];
				SpriteRenderComponent renderComp = SpriteRenderComponent(
					rendererJson["shaderVertexPath"].GetString(),
					rendererJson["shaderfragmentPath"].GetString(),
					rendererJson["texturePath"].GetString(),
					glm::vec3(rendererJson["colorX"].GetFloat(), rendererJson["colorY"].GetFloat(), rendererJson["colorZ"].GetFloat()));

				registry.addComponent<SpriteRenderComponent>(ent, std::move(renderComp));

				updateTexture(rendererJson["texturePath"].GetString(), ent);
			}
		}
	}
private:
	Registry& registry = Registry::instance();
	componentRegistry& ComponentRegistry = componentRegistry::instance();

	Document doc;
	Document::AllocatorType& allocator;

};

