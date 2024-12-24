#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imguiLoader.h"
#include "ComponentLoader.h"
#include "Registry.h"
#include "WejseRenderer.h"

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

	// Create a RapidJSON document object
	rapidjson::Document document;
	document.SetObject();  // Set the document type to an object

	// Create an allocator for managing memory (required by RapidJSON)
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	// Add some key-value pairs to the document
	document.AddMember("name", "John Doe", allocator);
	document.AddMember("age", 30, allocator);
	document.AddMember("isStudent", false, allocator);

	// Convert document to JSON string
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);  // Serialize the document to the writer

	// Output the resulting JSON string
	std::cout << buffer.GetString() << std::endl;  // {"name":"John Doe","age":30,"isStudent":false}

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


	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


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
