
#define _CRT_SECURE_NO_WARNINGS


#include "imguiLoader.h"






bool show_demo_window = true;


Registry& registry = Registry::instance();



ContentBroswer contentBrowser;
EntityPanel entityPanel;
InspectorPanel inspectorPanel;
ScenePanel scenePanel;

MeshRenderSystem meshRenderSystem;

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
	meshRenderSystem.meshRenderInitialisation();

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
	scenePanel.update();

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
	glEnable(GL_DEPTH_TEST);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
		// 1. Render opaque objects (no blending)
	meshRenderSystem.meshRenderRender();  // Render opaque meshes

	RenderUpdate(registry);  // Update the transparent objects



	// 3. Finish other operations like transforms
	UpdateTransform(registry);

	// Unbind the framebuffer
	scenePanel.unbind_framebuffer(); // Comment this out if rendering to the default framebuffer
	glDisable(GL_BLEND);


}



void CleanupImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


