

// ComponentLoader.h
#pragma once

#ifdef _WIN32
#ifdef COMPONENTLOADER_EXPORTS
#define COMPONENT_API __declspec(dllexport)  // When building the DLL
#else
#define COMPONENT_API __declspec(dllimport)  // When using the DLL
#endif
#else
#define COMPONENT_API  // Other platforms don't need this
#endif

#include "../CoreLib/Registry.h"



extern "C" COMPONENT_API void ScriptInitialise();

extern "C" COMPONENT_API void ScriptDestroy();