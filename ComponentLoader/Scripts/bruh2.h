#pragma once

#include "ScriptingAPI.h"
#include "ComponentFile.h"
#include "Registry.h"
#include "wejseRenderer.h"


class bruh2 : public Script
{
public:

	UPROPERTY
	float bruh = 0;

	UPROPERTY
	int speed = 1;

	void OnStart() override {  }
	void OnUpdate()override {
		Registry& registry = Registry::instance();
		auto entity = registry.getEntitiesWithComponent<bruh2>();

		for (auto ent : entity)
		{
			auto transform = registry.getComponent<TransformComponent>(ent);
			transform->translate.x +=  speed * static_cast<float>(WejseGetDT());
		}
	
	}

	RTTR_ENABLE(Script)
};

