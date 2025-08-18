
#pragma once

#include "ScriptingAPI.h"
#include "ComponentFile.h"
#include "Registry.h"
#include "wejseRenderer.h"

class asdfasdfasdf  : public Script
{
public:


	UPROPERTY
	int demo = 1;

	UPROPERTY
		int demo2 = 1;

	void OnStart() {
		std::cout << "hello";

	}

	void OnUpdate() {

	}


	void hello()
	{
	}
	RTTR_ENABLE(Script)
};

