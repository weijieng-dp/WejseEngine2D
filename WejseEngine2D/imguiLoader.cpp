
#define _CRT_SECURE_NO_WARNINGS


#include "imguiLoader.h"


#include "SceneManager.h"

#include "ContentBrowser.h"
#include "EntityPanel.h"
#include "InspectorPanel.h"
#include "ScenePanel.h"


bool show_demo_window = true;


Registry& registry = Registry::instance();

SceneManager sceneManager;


ContentBroswer contentBrowser;
EntityPanel entityPanel;
InspectorPanel inspectorPanel;
ScenePanel scenePanel , gameScene;

void InitializeImGui(GLFWwindow* window)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch



	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
	//InitialiseEntityPicking();
	scenePanel.initialise();
	gameScene.initialise();
	sceneManager.LoadScene("Assets/Scenes/scene1.txt");


}


void UpdateImGui()
{
	ImGuiIO& io = ImGui::GetIO();



	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// Disable ImGui keyboard capture
	io.WantCaptureKeyboard = false;

	// Clear previous keyboard states

	// Set frame time
	io.DeltaTime = WejseGetDT();

	inspectorPanel.update();


	//io.DisplaySize = ImVec2(
	//	static_cast<float>(1900),   // Width of the window
	//	static_cast<float>(1200)   // Height of the window
	//);



	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

}

void RenderImGui()
{

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(ScreenWidth, Screenheight);
	// Render ImGui UI
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);



	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.

	//EntityManager& manager = EntityManager::GetInstance();
	gameScene.render("game Scene");
	scenePanel.render("Scene Panel");

	contentBrowser.render();

	entityPanel.render();

	inspectorPanel.render();

	


	ImGui::End();

	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
	//	Entity selectedEntity = getObjectAtMousePosition(manager.Entities);
	//	if (&selectedEntity) {
	//		std::cout << "Selected Entity ID: " << selectedEntity.getId() << std::endl;
	//	}
	//}


	ImGui::Render();


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}


	scenePanel.bind_framebuffer(); // Comment this out if you are rendering to the default framebuffer

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		sceneManager.SaveScene("Assets/Scenes/scene1.txt");

	glClear(GL_COLOR_BUFFER_BIT);
	RenderUpdate(registry);
	UpdateTransform(registry);
	// Update entities
	//manager.UpdateEntities();

	// Unbind the framebuffer
	scenePanel.unbind_framebuffer(); // Comment this out if rendering to the default framebuffer

	gameScene.bind_framebuffer();

	glClear(GL_COLOR_BUFFER_BIT);
	RenderUpdate(registry);
	UpdateTransform(registry);

	gameScene.unbind_framebuffer();
}



void CleanupImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


