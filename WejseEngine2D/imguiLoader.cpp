
#define _CRT_SECURE_NO_WARNINGS


#include "imguiLoader.h"
#include "WejseRenderer.h"
#include "ObjectPicking.h"
#include "ComponentRegistry.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "selectionComponent.h"
#include "ComponentRegistry.h"


#include <iostream>

#include "RenderSystem.h"

bool show_demo_window = true;
unsigned int FBO;
unsigned int RBO;
unsigned int texture_id;


Registry& registry = Registry::instance();
componentRegistry& Componentregistry = componentRegistry::instance();

std::string selectedComponentstring;

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
	create_framebuffer();

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

	auto entwithselectcomponet = registry.getEntitiesWithComponent<selectionComponent>();
	for (auto entity : entwithselectcomponet) {
		auto selectComponet = registry.getComponent<selectionComponent>(entity);
		if (selectComponet->selected && selectedComponentstring != "")
		{
			Componentregistry.createComponent(selectedComponentstring, entity);
			selectedComponentstring = "";
		}
	}


	//io.DisplaySize = ImVec2(
	//	static_cast<float>(1900),   // Width of the window
	//	static_cast<float>(1200)   // Height of the window
	//);

	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void RenderImGui()
{

	ImGuiIO& io = ImGui::GetIO();
	// Render ImGui UI
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("My Scene", NULL, ImGuiWindowFlags_NoMove);

	const float window_width = ImGui::GetContentRegionAvail().x;
	const float window_height = ImGui::GetContentRegionAvail().y;

	// we rescale the framebuffer to the actual window size here and reset the glViewport 
	rescale_framebuffer(window_width, window_height);
	glViewport(0, 0, window_width, window_height);

	// we get the screen position of the window
	ImVec2 pos = ImGui::GetCursorScreenPos();

	// and here we can add our created texture as image to ImGui
	// unfortunately we need to use the cast to void* or I didn't find another way tbh
	ImGui::GetWindowDrawList()->AddImage(
		(void*)texture_id,
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + window_width, pos.y + window_height),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.

	//EntityManager& manager = EntityManager::GetInstance();



	if (ImGui::TreeNode("Entities"))
	{

		auto entitywithselectComponent = registry.getEntitiesWithComponent<selectionComponent>();
		for (auto entity : entitywithselectComponent)
		{
			//EncodeEntityID(manager.Entities[n]);


			auto selectComponent = registry.getComponent<selectionComponent>(entity);

			std::string entities = "entity" + std::to_string(entity);


			if (ImGui::Selectable(entities.c_str(), selectComponent->selected))
			{


				if (!ImGui::GetIO().KeyCtrl)  // Clear selection if CTRL is not held
				{
					// Deselect all entities when CTRL is not held
					for (auto entity2 : entitywithselectComponent)
					{
						auto selectedComponent = registry.getComponent<selectionComponent>(entity2);
						selectedComponent->selected = false;
					}
				}
				// Toggle the selection state for the current entity
				selectComponent->selected = !selectComponent->selected;
			}


		}

		if (ImGui::Button("Add Entities"))
		{
			auto entities = registry.createEntity();
			registry.addComponent<TransformComponent>(entities, {});
			Componentregistry.createComponent("Selection Component", entities);
		}
		ImGui::TreePop();
	}

	ImGui::Begin("Inspector");
	auto entitywithtransformComponent = registry.getEntitiesWithComponents<TransformComponent, selectionComponent>();
	for (auto entity : entitywithtransformComponent)
	{
		auto selectioncomponent = registry.getComponent<selectionComponent>(entity);
		if (selectioncomponent->selected)
		{
			auto tranformcomponent = registry.getComponent<TransformComponent>(entity);
			auto* rendercomponent = registry.getComponent<RenderComponent>(entity);

			if (io.KeysDown[ImGuiKey_Delete])
			{
				registry.destroyEntity(entity);
			}
			ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode("Component"))
			{
				ImGui::SeparatorText("Transform Component");


				float TranslationValue[] = { tranformcomponent->translate.x, tranformcomponent->translate.y };
				float scaleValue[] = { tranformcomponent->scale.x,tranformcomponent->scale.y };

				ImGui::DragFloat2("Position: ", TranslationValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
				ImGui::DragFloat("Rotation: ", &tranformcomponent->rotation, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
				ImGui::DragFloat2("Scale: ", scaleValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

				tranformcomponent->translate = glm::vec3(TranslationValue[0], TranslationValue[1], 0);
				tranformcomponent->scale = glm::vec3(scaleValue[0], scaleValue[1], 1);


				if (rendercomponent)
				{
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::SeparatorText("Render Component");

					// Ensure the string has enough space for input
					rendercomponent->TextureString.resize(128); // Adjust size as needed

					// Text input field
					if (ImGui::InputText("Input Text", &rendercomponent->TextureString[0], rendercomponent->TextureString.size() + 1)) {
						// Optional: Resize back to the actual length
						rendercomponent->TextureString.resize(strlen(rendercomponent->TextureString.c_str()));
					}

					// Show the current value of the input
					if (ImGui::Button("Change Texture"))
					{
						updateTexture(rendercomponent->TextureString, entity);
					}
					if (ImGui::Button("Delete Component")) {
						registry.removeComponent<RenderComponent>(entity);
					}
				}

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				if (ImGui::Button("Add Component")) {
					ImGui::OpenPopup("ComponentSelectorPopup");
				}

				if (ImGui::BeginPopup("ComponentSelectorPopup")) {
					ImGui::Text("Components");
					ImGui::Separator();

					for (const auto& componentName : Componentregistry.getComponentTypes()) {
						if (ImGui::Selectable(componentName.c_str())) {
							selectedComponentstring = componentName; // Update the selected component
						}
					}


					ImGui::EndPopup();
				}


				ImGui::TreePop();
			}


		}
	}


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
}



void CleanupImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}




void create_framebuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, Screenheight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);


	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, Screenheight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

// here we bind our framebuffer
void bind_framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

// here we unbind our framebuffer
void unbind_framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// and we rescale the buffer, so we're able to resize the window
void rescale_framebuffer(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

void SetImGuiFullScreen(GLFWwindow* window)
{
	// Get the primary monitor
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

	// Get the monitor's video mode (screen size)
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	// Set the window size to match the monitor's resolution
	glfwSetWindowSize(window, mode->width, mode->height);

	// Position the window at the top-left corner (optional if you want to control the position)
	glfwSetWindowPos(window, 0, 0);

	// Make sure the window is visible
	glfwShowWindow(window);

	// Optional: Set the ImGui window to match the window size
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)mode->width, (float)mode->height);
}
