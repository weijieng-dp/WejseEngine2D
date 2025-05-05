#pragma once

#include "ScriptingAPI.h"
#include "ComponentFile.h"
#include "Registry.h"
#include "wejseRenderer.h"

class test123 : public Script
{
public:
	UPROPERTY
	int speed = 1;

	void OnStart() {  }
	void OnUpdate() {
		Registry& registry = Registry::instance();
		auto entity = registry.getEntitiesWithComponent<test123>();

		for (auto ent : entity)
		{
			auto transform = registry.getComponent<TransformComponent>(ent);
			//transform->translate.x +=  speed * WejseGetDT();
			//std::cout << "hello" << std::endl;
			hello();
		}
	
	}


	void hello()
	{
		std::cout << "hello" << std::endl;

	}
	RTTR_ENABLE(Script)
};

