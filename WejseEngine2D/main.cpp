#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imguiLoader.h"
#include "ComponentLoader.h"
#include "Registry.h"
#include "WejseRenderer.h"
#include "SceneManager.h"

#include "RenderSystem.h"
#include "TransformSystem.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


#ifdef _DEBUG
bool debug = true;
#else
bool debug = false;
#endif 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{


	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	window = glfwCreateWindow(ScreenWidth, Screenheight, "Wejse2D Game Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make context current for the window
	glfwMakeContextCurrent(window);

	// Load OpenGL functions using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{

		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set the viewport size
	glViewport(0, 0, ScreenWidth, Screenheight);
	glfwSwapInterval(1);




	if(debug)
		InitializeImGui(window);

	Registry& registry = Registry::instance();
	

	ComponentInitialise();
	RenderInitialise();

	SceneManager sceneManager;

	sceneManager.LoadScene("Assets/Scenes/scene1.txt");
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


		if (!debug)
		{

			auto entitiesWithTransform = registry.getEntitiesWithComponent<RenderComponent>();
			for (auto entity : entitiesWithTransform) 
			{

				std::cout << "Entity with Transform" << entity << "\n";
			}
			UpdateTransform(registry);
			RenderUpdate(registry);
			// Update entities
			//manager.UpdateEntities();

		}
		else
		{
			// Start a new ImGui frame
			UpdateImGui();
			// Render ImGui interface
			RenderImGui();
			// Optionally, render to a framebuffer if necessary
			bind_framebuffer(); // Comment this out if you are rendering to the default framebuffer

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				sceneManager.SaveScene("Assets/Scenes/scene1.txt");

			glClear(GL_COLOR_BUFFER_BIT);
			RenderUpdate(registry);
			UpdateTransform(registry);
			// Update entities
			//manager.UpdateEntities();

			// Unbind the framebuffer
			unbind_framebuffer(); // Comment this out if rendering to the default framebuffer
		}
		// Poll events
		glfwPollEvents();

		// Since the window is hidden, we don't need to call glfwSwapBuffers(window)
		// But you can try calling it temporarily to see if rendering shows up
		glfwSwapBuffers(window); // Uncomment temporarily for debugging if needed
	}

	// Cleanup
	//manager.FreeEntity();
	registry.DestroyAllEntities();
	RenderCleanUp();
	glfwTerminate();
	if (debug)
		CleanupImGui();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
