#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imguiLoader.h"
#include "ComponentLoader.h"
#include "Registry.h"
#include "SceneManager.h"
#include "WejseRenderer.h"

#include "SpriteRenderSystem.h"
#include "MeshRenderSystem.h"
#include "TransformSystem.h"







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

	SceneManager sceneManager;


	if(debug)
		InitializeImGui(window);
	else
		sceneManager.LoadScene("Assets/Scenes/scene1.txt");

	Registry& registry = Registry::instance();
	MeshRenderSystem meshRenderSystem;


	ComponentInitialise();
	RenderInitialise();

	meshRenderSystem.meshRenderInitialisation();
	//sceneManager.LoadScene("Assets/Scenes/scene1.txt");

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


		if (!debug)
		{
			glEnable(GL_DEPTH_TEST);


			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			UpdateTransform(registry);
			meshRenderSystem.meshRenderRender();
			RenderUpdate(registry);
			// Update entities

		}
		else
		{
			// Start a new ImGui frame
			UpdateImGui();
			// Render ImGui interface
			RenderImGui();
			// Optionally, render to a framebuffer if necessary
		
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
