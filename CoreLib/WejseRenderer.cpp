#include "pch.h"
#include "WejseRenderer.h"


float ScreenWidth = 1600;
float Screenheight = 900;

GLFWwindow* globalWindow;

std::string ScenePlaying;
bool isPlaying = false;
bool isPaused = false;

#ifdef _DEBUG
bool debug = true;
#else
bool debug = false;
#endif 


static double lastFrameTime = 0.0f;
static double currentFrameTime;
static double DeltaTime;
static double timescale = 1;

void setScale(float timeScale)
{
	timescale = timeScale;
}

 double WejseGetDT()
{
	return DeltaTime * timescale;
}

void updateDT(double time)
{
	currentFrameTime = time;
	DeltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

}

bool GetKeyTriggered(int key)
{
	static std::unordered_map<int, int> previousKeyStates; // Store previous states per key.

	// Get current state of the key.
	int keyState = glfwGetKey(globalWindow, key);

	// Check if the key was just pressed (transition from RELEASE to PRESS).
	bool isTriggered = (keyState == GLFW_PRESS && previousKeyStates[key] == GLFW_RELEASE);

	// Update the previous key state for the current key.
	previousKeyStates[key] = keyState;

	return isTriggered;
}

bool GetButtonTriggered(int Button)
{
	static int previousButton[GLFW_MOUSE_BUTTON_LAST + 1] = { GLFW_RELEASE };

	// Get current state of the button
	int keyState = glfwGetMouseButton(globalWindow, Button);

	// Detect if the button was just pressed
	bool isTriggered = (keyState == GLFW_PRESS && previousButton[Button] == GLFW_RELEASE);

	// Update previous state
	previousButton[Button] = keyState;

	return isTriggered;
}