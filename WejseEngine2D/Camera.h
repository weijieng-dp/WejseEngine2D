#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "WejseRenderer.h"
#include "GLFW/glfw3.h"

class camera
{
public:
	static camera& instance() {
		static camera instance;  // Created lazily
		return instance;
	}

	// Prevent copying and assignment
	camera(const camera&) = delete;
	camera& operator=(const camera&) = delete;

	glm::mat4 getviewmatrix()
	{
		camPos = glm::mat4(1);
		return glm::translate(camPos,position);
	}

	void processMovement()
	{
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			position += glm::vec3(0,-speed,0);
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			position += glm::vec3(0, speed, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			position += glm::vec3(speed,0, 0);

		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			position += glm::vec3(-speed,0, 0);
		}
	}
private:
	camera() {}
	glm::mat4 camPos = glm::mat4(1.0);
	glm::vec3 position = glm::vec3(0,0,0);
	float speed = 1;

};