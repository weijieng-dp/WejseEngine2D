#include <glad/glad.h>
#include <GLFW/glfw3.h>

// add headers that you want to pre-compile here
#include "Windows.h"
#include "ComponentLoader.h"
#include "imguiLoader.h"
#include "Registry.h"
#include "SceneManager.h"
#include "WejseRenderer.h"
#include "RenderSystem.h"
#include "TransformSystem.h"
#include <iostream>
#include <rttr/library.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Windows.h"
#include "filewatch/FileWatch.hpp"
#include "Camera.h"




std::string compileCmd;
HMODULE hDll;
bool isHotReloading = false;
bool FileModified;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int systemInit();
void recompilescript();

bool isInitialized = false;

typedef void (*ScriptInitialiseFunc)();
typedef void (*ScriptDestroy)();

rttr::library lib("ComponentLoader"); // file suffix is not needed, will be automatically appended


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!systemInit())
		return -1;

	//if(debug)


#ifdef _DEBUG

	filewatch::FileWatch<std::string> watcher("../ComponentLoader/Scripts", [](const std::string& path, const filewatch::Event change_type) {
		if (change_type == filewatch::Event::modified) {
			// Run recompilation only if the app is focused
			std::cout << "File modified, recompiling...\n";
			FileModified = true;
		}

		if (change_type == filewatch::Event::added) {
			// Run recompilation only if the app is focused
			std::cout << "File modified, recompiling...\n";
			FileModified = true;

		}
		if (change_type == filewatch::Event::removed) {
			// Run recompilation only if the app is focused
			std::cout << "File modified, recompiling...\n";
			FileModified = true;

		}
		});


	ScanDirectoryForClasses("../ComponentLoader/Scripts/");
	initializeImGui(globalWindow);

	compileCmd =
		"call \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" && "
		"msbuild ../ComponentLoader/ComponentLoader.vcxproj /p:TargetName=ComponentLoader /p:Configuration=Debug /p:Platform=x64 /p:OutDir=\"..//x64//Debug//\" "
		;

	int result = system(compileCmd.c_str());

	if (result == 0)
	{
		printf("Compilation succeeded!\n");
		// Example: load function by name (not useful for RTTR unless exposing custom API


	}
	else
		printf("Compilation failed with code %d\n", result);
#endif


	hDll = LoadLibraryA("ComponentLoader.dll");
	if (!hDll)
	{
		std::cerr << "Failed to load DLL\n";
		return -1;
	}

	if (lib.load())
		std::cout << "succedded" << std::endl;
	else
		std::cout << "failed: " << lib.get_error_string() << std::endl;


	ScriptInitialiseFunc myFunc = nullptr;



	myFunc = (ScriptInitialiseFunc)GetProcAddress(hDll, "ScriptInitialise");
	if (myFunc)
		myFunc();
	else
		std::cerr << "Failed to find ScriptInitialise\n";

	SceneManager sceneManager;



	Registry& registry = Registry::instance();
	componentRegistry& cr = componentRegistry::instance();

	ComponentInitialise();
	renderInitialise();
	meshRenderInitialisation();


	if (!debug)
		sceneManager.LoadScene("Assets/Scenes/Credit_pg1 - Copy.txt");


	while (!glfwWindowShouldClose(globalWindow))
	{
		glfwSetWindowTitle(globalWindow,std::to_string(1.f/WejseGetDT()).c_str());
		updateDT(glfwGetTime());
		if (glfwGetKey(globalWindow, GLFW_KEY_SPACE))
		{
			for (int i = 0; i < 1000; i++)
			{
				Registry::Entity ent = registry.createEntity();
				registry.addComponent<TransformComponent>(ent, {});
				cr.createComponent("Selection Component", ent);
				std::cout << ent << std::endl;
			}
			
		}
#ifdef _DEBUG
		if (GetKeyTriggered(GLFW_KEY_EQUAL) && glfwGetKey(globalWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			auto Cameraent = registry.getEntitiesWithComponent<CameraComponent>();


			if (!Cameraent.empty())
			{
				if (debug)
				{
					sceneManager.SaveScene(ScenePlaying.c_str());

					auto ent = registry.getAllEntity();
					for (auto entity : ent)
					{
						auto rttrcomp = registry.getAllComponentsRTTR(entity);
						for (auto const& obj : rttrcomp)
						{
							rttr::method func = obj.get_type().get_method("OnStart");
							if (func)
							{
								func.invoke(obj);
							}
						}
					}
				}
				if (!debug)
				{



					std::cout << "load scene";
					sceneManager.LoadScene(ScenePlaying.c_str());

				}
				debug = !debug;

			}
			else
			{
				std::cout << "Camera Coponent is Not Attached" << std::endl;
			}
		}
#endif
		if (GetKeyTriggered(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(globalWindow, true);
		}

		if (!debug)
		{
			int width, height;
			glfwGetWindowSize(globalWindow, &width, &height);
			glViewport(0, 0, width, height);
			ScreenWidth = static_cast<float>(width);
			Screenheight = static_cast<float>(height);
			//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



			auto ent = registry.getAllEntity();
			for (auto entity : ent)
			{
				auto rttrcomp = registry.getAllComponentsRTTR(entity);
				for (auto const& obj : rttrcomp)
				{
					rttr::method func = obj.get_type().get_method("OnUpdate");
					if (func)
					{
						func.invoke(obj);
					}
				}
			}


			UpdateTransform(registry);
			//physics update system
			//collider update system
			// 
			//rendering
			meshRenderRender();
			renderUpdate(registry);
			fontRenderer();



		}
		else
		{

			//));

			// Optionally, render to a framebuffer if necessary

			if (FileModified
				&& glfwGetWindowAttrib(globalWindow, GLFW_FOCUSED))
			{

				recompilescript();
				FileModified = false;

			}


			if (isPlaying)
			{
				if (isPaused)
				{
					auto ent = registry.getAllEntity();
					if (!isInitialized)
					{
						isInitialized = true;
						for (auto entity : ent)
						{
							auto rttrcomp = registry.getAllComponentsRTTR(entity);
							for (auto const& obj : rttrcomp)
							{
								rttr::method func = obj.get_type().get_method("OnStart");
								if (func)
								{
									func.invoke(obj);
								}
							}
						}

					}

					for (auto entity : ent)
					{
						auto rttrcomp = registry.getAllComponentsRTTR(entity);
						for (auto const& obj : rttrcomp)
						{
							rttr::method func = obj.get_type().get_method("OnUpdate");
							if (func)
							{
								func.invoke(obj);
							}
						}
					}
				}
			}
			else
			{
				if (isInitialized)
				{
					sceneManager.LoadScene(ScenePlaying.c_str());
					isInitialized = false;
				}
			}


			// Trigger whatever you need here
					// Render ImGui interface
			if (!isHotReloading)
			{
				updateImGui();
				renderImGui();
			}
		}
		// Poll events
		glfwPollEvents();

		// Since the window is hidden, we don't need to call glfwSwapBuffers(window)
		// But you can try calling it temporarily to see if rendering shows up
		glfwSwapBuffers(globalWindow); // Uncomment temporarily for debugging if needed
	}
	// Cleanup
	//manager.FreeEntity();
	registry.destroyAllEntities();
	RenderCleanUp();
	glfwTerminate();
	//if (debug)
	cleanupImGui();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	ScreenWidth = static_cast<float>(width);
	ScreenWidth = static_cast<float>(height);
	glViewport(0, 0, width, height);
}


int systemInit()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return 0;
	}

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	globalWindow = glfwCreateWindow(static_cast<int>(ScreenWidth), static_cast<int>(Screenheight), "Book It", NULL, NULL);
	if (globalWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 0;
	}

	// Make context current for the window
	glfwMakeContextCurrent(globalWindow);

	// Load OpenGL functions using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{

		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}

	// Set the viewport size
	glViewport(0, 0, static_cast<GLsizei>(ScreenWidth), static_cast<GLsizei>(Screenheight));

	glfwSwapInterval(1);
	return 1;
}

void recompilescript()
{
	Registry& registry = Registry::instance();
	SceneManager sceneManager;


	ScanDirectoryForClasses("../ComponentLoader/Scripts");

	compileCmd =
		"call \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" && "
		"msbuild ../ComponentLoader/ComponentLoader.vcxproj  /p:TargetName=ComponentLoader_temp /p:Configuration=Debug /p:Platform=x64 /p:OutDir=\"..//x64//Debug//\" /m";

	int result = system(compileCmd.c_str());

	if (result == 0)
	{
		ScriptDestroy myFunc2 = (ScriptDestroy)GetProcAddress(hDll, "ScriptDestroy");
		if (myFunc2)
			myFunc2();
		myFunc2 = nullptr;
		int entnum = 0;




		for (auto ent : registry.getEntitiesWithComponent<selectionComponent>())
		{
			if (registry.getComponent<selectionComponent>(ent)->selected)
			{
				entnum = ent;
			}
		}

		sceneManager.SaveScene("Assets/temp.json");
		registry.destroyAllEntities();
		registry.restartEntityCount();
		registry.clearAllComponentStorages();

		if (lib.is_loaded())
		{
			lib.unload();
		}

		FreeLibrary(hDll);


		std::cout << "Unload successful\n";

		Sleep(100);  // Add a small delay to allow the OS to free the DLL

		system("copy \"..\\x64\\Debug\\ComponentLoader_temp.dll\" \"..\\x64\\Debug\\ComponentLoader.dll\"");
		system("copy \"..\\x64\\Debug\\ComponentLoader_temp.lib\" \"..\\x64\\Debug\\ComponentLoader.lib\"");
		system("copy \"..\\x64\\Debug\\ComponentLoader_temp.exp\" \"..\\x64\\Debug\\ComponentLoader.exp\"");


		// Scan directory for changes


		// Run the compile and copy commands
		//int result = system(compileCmd.c_str());


		// Load the new DLL
		hDll = LoadLibraryA("ComponentLoader.dll");
		if (!hDll)
		{
			std::cerr << "Failed to load DLL\n";
			return;
		}
		if (!lib.is_loaded())
		{
			if (lib.load())
			{
			}
			else
			{
				std::cout << lib.get_error_string() << std::endl;
			}
		}

		// If compilation was successful, initialize the script

		printf("Compilation succeeded!\n");
		ScriptInitialiseFunc myFunc = (ScriptInitialiseFunc)GetProcAddress(hDll, "ScriptInitialise");
		if (myFunc)

			myFunc();
		else
			std::cerr << "Failed to find ScriptInitialise\n";



		sceneManager.LoadScene("Assets/temp.json");

		if (entnum != 0)
		{
			registry.getComponent<selectionComponent>(entnum)->selected = true;

		}
	}
}