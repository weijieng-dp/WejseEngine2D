#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Entity.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "Registry.h"

#define M_PI 3.14159265358979323846

extern float ScreenWidth;
extern float Screenheight;

extern GLFWwindow* window;
extern bool debug;

static GLuint QuadVAO,QuadVBO;
static GLuint TriVAO, TriVBO;
static GLuint LineVAO, LineVBO;
static GLuint BoxVAO, BoxVBO,BoxEBO;
static GLuint circleVAO, circleVBO;
static GLuint squareVAO, squareVBO;



inline void initialiseQuad()
{
	float vertices[] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   // top left

		0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left
	};

	glGenBuffers(1, &QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &QuadVAO);
	glBindVertexArray(QuadVAO);

	// Set up vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up texture coordinates attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

inline void initialiseSquare()
{
	float vertices[] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,    // top right
		0.5f, -0.5f, 0.0f,    // bottom right
		-0.5f,  0.5f, 0.0f,   // top left

		0.5f, -0.5f, 0.0f,    // bottom right
		-0.5f, -0.5f, 0.0f,     // bottom left
		-0.5f,  0.5f, 0.0f,     // top left
	};

	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);

	// Set up vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

inline void initialiseTriangle()
{

	float vertices[] = {
		// positions         
		 0.0f,  0.5f, 0.0f,   
		-0.5f, -0.5f, 0.0f,   
		 0.5f, -0.5f, 0.0f   

	};


	glGenBuffers(1, &TriVBO);


	glGenVertexArrays(1, &TriVAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(TriVAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, TriVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
}

inline void initialiseWireFrameBox() {
	float boxVertices[] = {
		0.5f,  0.5f, 0.0f,   // top right
		 0.5f, -0.5f, 0.0f,   // bottom right
		-0.5f,  -0.5f, 0.0f,    // top left 

		-0.5f, 0.5f, 0.0f,  // bottom right

	};

	glGenBuffers(1, &BoxVBO);


	glGenVertexArrays(1, &BoxVAO);
	glBindVertexArray(BoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}




inline void initialiseLines()
{

	float vertices[] = {
		// positions         
		 0.5f,  0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
	};


	glGenBuffers(1, &LineVBO);



	glGenVertexArrays(1, &LineVAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(LineVAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, LineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
}

inline void initialiseCircle()
{
	std::vector<float> vertices;

	// Add the center vertex
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	// Add the circumference vertices
	for (int i = 0; i <= 32; ++i) {
		float angle = (2.0f * M_PI * i) / 32;
		float x = 0.0f + .5f * cos(angle);
		float y = 0.0f + .5f * sin(angle);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);
	}

	// Create and bind a VAO and VBO
	glGenVertexArrays(1, &circleVAO);
	glGenBuffers(1, &circleVBO);

	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

inline void initialiseShapes()
{
	initialiseCircle();
	initialiseLines();
	initialiseQuad();
	initialiseTriangle();
	initialiseWireFrameBox();
}

inline void drawsquare()
{
	glBindVertexArray(squareVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

inline void drawWireframeBox() {
	glBindVertexArray(BoxVAO);
	glDrawArrays(GL_LINE_LOOP, 0,4); // Draw lines
	glBindVertexArray(0);
}


inline void drawCircle() {
	glBindVertexArray(circleVAO);
	glDrawArrays(GL_LINE_LOOP, 1, 32);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

inline void drawfilledCircle() {


	// Draw the circle
	glBindVertexArray(circleVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 34);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

inline void DrawQuads()
{
	glBindVertexArray(QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

inline void DrawTriangle()
{
	glBindVertexArray(TriVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

inline void drawLine() {
	glBindVertexArray(LineVAO);
	glDrawArrays(GL_LINES, 0, 2); // Only two vertices for a line
	glBindVertexArray(0);
}


inline void RenderCleanUp()
{
	glDeleteVertexArrays(1, &QuadVAO);
	glDeleteBuffers(1, &QuadVBO);
	glDeleteVertexArrays(1, &TriVAO);
	glDeleteBuffers(1, &TriVBO);
	glDeleteVertexArrays(1, &LineVAO);
	glDeleteBuffers(1, &LineVBO);
	glDeleteVertexArrays(1, &circleVAO);
	glDeleteBuffers(1, &circleVBO);
	glDeleteVertexArrays(1, &BoxVAO);
	glDeleteBuffers(1, &BoxVBO);
}



inline float WejseGetDT()
{
	static float lastFrameTime = 0.0f;
	float currentFrameTime = glfwGetTime();
	float DeltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	return DeltaTime;
}