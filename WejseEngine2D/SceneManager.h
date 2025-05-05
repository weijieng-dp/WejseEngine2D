#pragma once
//---------------------------------------------------------
// @file SceneManager.h
// @project name: Book-It
// @authors Ng wei jie (weijie.ng)
// @brief This file declares function that handles serialization
// and deserialization of entities and their components using RapidJSON
// and RTTR for reflection. It saves and loads scenes, dynamically serializes
// component properties, and restores entities while maintaining relationships
// . The system ensures proper type handling for various data types like integers,
// floats, strings, vectors, and entity references.n.
// 
//
// Copyright © 2025 DigiPen, All rights reserved.
//---------------------------------------------------------


#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Registry.h"
#include "rapidjson/prettywriter.h"


#include "componentfile.h"


#include "RenderSystem.h"

#include "rttr/type.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace rapidjson;

class SceneManager
{
public:
	SceneManager() : allocator(doc.GetAllocator()) {}

	void SaveScene(const char* filepath);
	void LoadScene(const char* filepath);

	Value Serialize();
	void Deserialize(const Value& entities);

	void DynamicSerializer(rapidjson::Value& entJson, EntityRegistry::Entity ent);
	void DynamicDeserializer(const rapidjson::Value& entJson, EntityRegistry::Entity ent);


private:
	Registry& registry = Registry::instance();
	componentRegistry& ComponentRegistry = componentRegistry::instance();

	Document doc;
	Document::AllocatorType& allocator;

};

