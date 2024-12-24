#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Entity.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Registry.h"


extern float ScreenWidth;
extern float Screenheight;
extern GLFWwindow* window;

inline void initialiseOpenGL(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO)
{

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


//inline void SetTransformMatrix(Registry registry) {
//	if (entity.hasComponent<RenderComponent>() && entity.hasComponent<TransformComponent>()) {
//		// Get components
//		auto& renderComponent = *entity.getComponent<RenderComponent>();
//		auto& transformComponent = *entity.getComponent<TransformComponent>();
//
//		// Retrieve model matrix from TransformComponent
//		glm::mat4 model = transformComponent.transform;
//		glm::mat4 view = glm::mat4(1.0f);
//		glm::mat4 projection = glm::ortho(-ScreenWidth/2.0f, ScreenWidth/2.0f, -Screenheight/2.0f, Screenheight/2.0f, -1.0f, 1.0f);
//
//
//		// Activate the shader
//		Shader& shader = *renderComponent.shader;
//
//		// Set matrices in the shader
//		shader.setMat4("model", model);
//		shader.setMat4("view", view);
//		shader.setMat4("projection", projection);
//
//	}
//}


inline float WejseGetDT()
{
	static float lastFrameTime = 0.0f;
	float currentFrameTime = glfwGetTime();
	float DeltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	return DeltaTime;
}