#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"



#include "Registry.h"

#ifdef _WIN32
#ifdef REGISTRY_EXPORTS
#ifndef REGISTRY_API
#define REGISTRY_API __declspec(dllexport)  // When building the DLL
#endif

#else
#ifndef REGISTRY_API
#define REGISTRY_API __declspec(dllimport)  // When using the DLL
#endif
#endif
#else
#define REGISTRY_API  // Other platforms don't need this
#endif

#define M_PI 3.14159265358979323846
extern REGISTRY_API float ScreenWidth;
extern REGISTRY_API float Screenheight;

extern REGISTRY_API GLFWwindow* globalWindow;
extern REGISTRY_API bool debug;
extern REGISTRY_API std::string ScenePlaying;
extern REGISTRY_API bool isPlaying;
extern REGISTRY_API bool isPaused;

extern REGISTRY_API double WejseGetDT();

extern REGISTRY_API void updateDT(double time);
extern REGISTRY_API void   setScale(float timeScale);

// Check if a key is triggered (pressed down in this frame only).
extern REGISTRY_API bool  GetKeyTriggered(int key);

extern  REGISTRY_API bool  GetButtonTriggered(int Button);
