#pragma once

#include "ScriptingAPI.h"
#include "ComponentFile.h"
#include "Registry.h"
#include "wejseRenderer.h"


class bruh : public Script
{
public:

	UPROPERTY
	float bruh2 = 0;

	UPROPERTY
	int speed = 1;

	void OnStart() {
	
	}

	void OnUpdate() {
		Registry& registry = Registry::instance();
		auto entity = registry.getEntitiesWithComponent<bruh>();

		for (auto ent : entity)
		{
			auto transform = registry.getComponent<TransformComponent>(ent);
			transform->translate.x +=  speed * static_cast<float>(WejseGetDT());
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

