
#define _CRT_SECURE_NO_WARNINGS

#include "imguiLoader.h"






bool show_demo_window = true;


static Registry& registry = Registry::instance();
static componentRegistry& Componentregistry = componentRegistry::instance();
static camera& cam = camera::instance();




// Inspector Panel
/**************************************************************************************************/

std::string selectedComponentstring;


//
//
//// Render function to display properties dynamically
void renderDynamicUI(EntityRegistry::Entity ent)
{

	auto components = registry.getAllComponentsRTTR(ent);


	//auto comp = registry.getComponent<t>(ent);
	// Get the type of the object dynamically
	for (auto& instance : components)
	{

		rttr::type obj_type = rttr::type::get(instance);

		if (!obj_type.is_valid())
		{
			std::cout << "not valid" << std::endl;
			continue;
		}

		if (obj_type == rttr::type::get<NameComponent>() || obj_type == rttr::type::get<ActiveComponent>()
			|| obj_type == rttr::type::get<AnchorComponent>() || obj_type == rttr::type::get<selectionComponent>()
			|| obj_type == rttr::type::get<selectionComponent>())
			continue;
		ImGui::SeparatorText(obj_type.get_name().to_string().c_str());

		// Iterate through the object's properties
		for (auto& prop : obj_type.get_properties())
		{
			std::string prop_name = prop.get_name().to_string();
			auto value = prop.get_value(instance);

			if (value.is_type<int>()) {
				int int_value = value.get_value<int>();
				if (ImGui::InputInt(prop_name.c_str(), &int_value)) {
					prop.set_value(instance, int_value);  // Update the object property
				}
			}
			else if (value.is_type<std::string>() && prop_name == "shape")
			{


				static std::string items[] = { "square", "triangle", "circle","line" };
				std::string currentItem = value.get_value<std::string>();

				if (ImGui::BeginCombo("mesh", currentItem.c_str())) {
					for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
						bool isSelected = (currentItem == items[i]);
						if (ImGui::Selectable(items[i].c_str(), isSelected)) {
							std::cout << prop.set_value(instance, items[i]);
							currentItem = items[i];
						}
					}
					ImGui::EndCombo();
				}
			}
			else if (value.is_type<std::string>() && prop_name == "Texture")
			{
				std::string string_value = value.get_value<std::string>();
				// Ensure that the string has enough space for input
				if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
					// Update the object property with the new string length (truncated to fit)
					string_value.resize(strlen(string_value.c_str()));
					prop.set_value(instance, string_value);  // Update the object property
				}

				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const char* path = static_cast<const char*>(payload->Data);
						if (path)
						{
							updateTexture(path, ent);
							string_value = path;
						}
					}
					ImGui::EndDragDropTarget();
				}

			}
			else if (value.is_type<std::string>() && prop_name == "font")
			{
				std::string string_value = value.get_value<std::string>();
				string_value.resize(128);
				// Ensure that the string has enough space for input
				if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
					// Update the object property with the new string length (truncated to fit)
					string_value.resize(strlen(string_value.c_str()));
					prop.set_value(instance, string_value);  // Update the object property
				}

				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const char* path = static_cast<const char*>(payload->Data);
						if (path)
						{
							fontUpdate(path, ent);
							string_value = path;
						}
					}
					ImGui::EndDragDropTarget();
				}

			}
			//else if (value.is_type<std::string>() && prop_name == "File Path")
			//{
			//	std::string string_value = value.get_value<std::string>();
			//	string_value.resize(128);
			//	// Ensure that the string has enough space for input
			//	if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
			//		// Update the object property with the new string length (truncated to fit)
			//		string_value.resize(strlen(string_value.c_str()));
			//		prop.set_value(instance, string_value);  // Update the object property
			//	}

			//	if (ImGui::BeginDragDropTarget()) {
			//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			//			const char* path = static_cast<const char*>(payload->Data);
			//			if (path)
			//			{
			//				LoadSound(path, ent);
			//				string_value = path;
			//			}
			//		}
			//		ImGui::EndDragDropTarget();
			//	}

			//}
			else if (value.is_type<std::string>()) {
				std::string string_value = value.get_value<std::string>();
				string_value.resize(128);
				// Ensure that the string has enough space for input
				if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
					// Update the object property with the new string length (truncated to fit)
					string_value.resize(strlen(string_value.c_str()));
					prop.set_value(instance, string_value);  // Update the object property
				}
			}
			else if (value.is_type<glm::vec3>() && prop_name == "color") {
				glm::vec3 vec_value = value.get_value<glm::vec3>();
				if (ImGui::ColorEdit3(prop_name.c_str(), &vec_value[0])) {
					prop.set_value(instance, vec_value);  // Update the object property
				}
			}
			else if (value.is_type<glm::vec3>()) {
				glm::vec3 vec_value = value.get_value<glm::vec3>();
				if (ImGui::DragFloat3(prop_name.c_str(), &vec_value[0], 0.1f)) {
					prop.set_value(instance, vec_value);  // Update the object property
				}
			}
			else if (value.is_type<float>()) {
				float float_value = value.get_value<float>();
				if (ImGui::DragFloat(prop_name.c_str(), &float_value, 0.1f)) {
					prop.set_value(instance, float_value);  // Update the object property
				}
			}
			else if (value.is_type<bool>()) {
				bool bool_value = value.get_value<bool>();
				if (ImGui::Checkbox(prop_name.c_str(), &bool_value)) {
					prop.set_value(instance, bool_value);  // Update the object property
				}
			}
			else if (value.is_type<EntityRegistry::Entity>())
			{
				std::string string_value = "";
				auto nameComponent = registry.getComponent<NameComponent>(value.get_value<EntityRegistry::Entity>());
				if (nameComponent)
				{
					string_value = nameComponent->name;
				}

				// Ensure that the string has enough space for input
				if (ImGui::InputText(prop_name.c_str(), &string_value[0], string_value.capacity())) {
					// Update the object property with the new string length (truncated to fit)
					string_value.resize(strlen(string_value.c_str()));
					//prop.set_value(comp, string_value);  // Update the object property
				}

				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
						auto entity = static_cast<EntityRegistry::Entity*>(payload->Data);
						if (entity)
						{
							std::cout << *entity << std::endl;
							prop.set_value(instance, *entity);
							nameComponent = registry.getComponent<NameComponent>(*entity);
							string_value = nameComponent->name;
						}
					}
					ImGui::EndDragDropTarget();
				}

			}
		}
		if (obj_type != rttr::type::get<TransformComponent>())
		{
			if (ImGui::Button(("Delete Component ##" + obj_type.get_name().to_string()).c_str())) {
				std::cout << "Delete button clicked for component: " << obj_type.get_name().to_string() << std::endl;
				if (obj_type == rttr::type::get<lightComponent>()) registry.getComponent<lightComponent>(ent)->lightIntensity = 0;
				registry.removeComponent(ent, obj_type);
			}

		}
		ImGui::Dummy(ImVec2(0, 10));

	}
}

void displayComponentNode(EntityRegistry::Entity entity)
{
	if (ImGui::TreeNode("Component"))
	{

		renderDynamicUI(entity);

		ImGui::TreePop();
	}
}

void componentPopup()
{
	if (ImGui::BeginPopup("ComponentSelectorPopup")) {
		ImGui::Text("Components");
		ImGui::Separator();

		//for (const auto& componentName : Componentregistry.getComponentTypes()) {
		//	if (ImGui::Selectable(componentName.c_str())) {
		//		selectedComponentstring = componentName; // Update the selected component
		//	}
		//}
		rttr::type base = rttr::type::get<Script>();

		for (const auto& componentName : base.get_derived_classes()) {
			if (ImGui::Selectable(componentName.get_name().to_string().c_str())) {
				selectedComponentstring = componentName.get_name().to_string(); // Update the selected component
			}
		}

		ImGui::EndPopup();
	}
}

void addComponentButton()
{
	if (ImGui::Button("Add Component")) {
		ImGui::OpenPopup("ComponentSelectorPopup");
	}

	componentPopup();
}



void displayEntityName(EntityRegistry::Entity entity)
{
	auto nameComponent = registry.getComponent<NameComponent>(entity);
	std::string entityName = nameComponent->name;
	// Resize the entity name to handle up to 128 characters (if necessary)
	entityName.resize(128);
	std::string label = "Entity##" + std::to_string(entity);

	if (ImGui::InputText(label.c_str(), &entityName[0], entityName.size() + 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (!entityName.empty()) {
			entityName.resize(strlen(entityName.c_str()));
			nameComponent->name = entityName;


			std::cout << "Updated Entity Name: " << entityName << std::endl;

		}
	}
}


void displayActive(EntityRegistry::Entity entity)
{
	auto checkbox = registry.getComponent<ActiveComponent>(entity);
	if (checkbox)
	{
		ImGui::Checkbox("##ActiveComponent", &checkbox->isActive);


	}
}


void setAnchorEntity(EntityRegistry::Entity entity)
{
	auto anchorComp = registry.getComponent<AnchorComponent>(entity);
	if (anchorComp)
	{
		static std::string items[] = { "Center", "Left", "Right" };
		std::string currentItem = anchorComp->AnchoredTo;

		if (ImGui::BeginCombo("Anchor", currentItem.c_str())) {
			for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
				bool isSelected = (currentItem == items[i]);
				if (ImGui::Selectable(items[i].c_str(), isSelected)) {
					currentItem = items[i];
					anchorComp->AnchoredTo = items[i].c_str();
				}
			}
			ImGui::EndCombo();
		}
	}
}

void handleDeleteEntity(EntityRegistry::Entity entity)
{
	ImGuiIO& io = ImGui::GetIO();

	if (io.KeysDown[ImGuiKey_Delete])
	{
		registry.destroyEntity(entity);
	}
}


void inspectorPanelUpdate()
{
	auto entwithselectcomponet = registry.getEntitiesWithComponent<selectionComponent>();
	for (auto entity : entwithselectcomponet) {
		auto selectComponet = registry.getComponent<selectionComponent>(entity);
		if (selectComponet->selected && !selectedComponentstring.empty())
		{
			Componentregistry.createComponent(selectedComponentstring, entity);
			selectedComponentstring = "";

		}
	}
}

void inspectorPanelRender()
{
	rttr::type transformType = rttr::type::get<TransformComponent>();

	ImGui::Begin("Inspector");
	auto entitywithtransformComponent = registry.getEntitiesWithComponents<TransformComponent, selectionComponent>();
	for (auto entity : entitywithtransformComponent)
	{
		auto selectioncomponent = registry.getComponent<selectionComponent>(entity);
		if (selectioncomponent->selected)
		{
			displayActive(entity);
			ImGui::SameLine();
			displayEntityName(entity);

			setAnchorEntity(entity);

			ImGui::SetNextItemOpen(true);
			displayComponentNode(entity);

			handleDeleteEntity(entity);

			ImGui::Dummy(ImVec2(11, 0));
			ImGui::SameLine();
			addComponentButton();

		}

	}



	ImGui::End();

}

/**************************************************************************************************/



// Entities Panel
/**************************************************************************************************/

static std::vector<rttr::instance> CopiedEntity;

void addEntitiesButton()
{
	if (ImGui::Button("Add Entities"))
	{
		auto entities = registry.createEntity();
		registry.addComponent<TransformComponent>(entities, {});
		Componentregistry.createComponent("Selection Component", entities);
	}
}



void dragDropEntitySource(EntityRegistry::Entity& entity)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("Entity", &entity, sizeof(entity));
		ImGui::Text("Dragging: %s", registry.getComponent<NameComponent>(entity)->name.c_str());
		ImGui::EndDragDropSource();
	}
}



void setEntitySelectFlag(EntityRegistry::Entity entity, bool flag)
{
	auto selectedComponent = registry.getComponent<selectionComponent>(entity);
	if (selectedComponent)
		selectedComponent->selected = flag;
}



void selectEntity(std::vector<EntityRegistry::Entity> entities)
{
	if (!ImGui::GetIO().KeyCtrl)  // Clear selection if CTRL is not held
	{
		// Deselect all entities when CTRL is not held
		for (auto entity : entities)
		{
			setEntitySelectFlag(entity, false);
		}
	}
}


std::string entityNamePanel(EntityRegistry::Entity entity)
{
	std::string entities = registry.getComponent<NameComponent>(entity)->name + "##" + std::to_string(entity);
	return entities;
}

void copyEntity(EntityRegistry::Entity const& originalEntity)
{
	auto allComps = registry.getAllComponentsRTTR(originalEntity);

	CopiedEntity.clear();

	for (auto const& comps : allComps)
	{
		CopiedEntity.push_back(comps);
	}
}

void pasteEntity()
{
	auto ent = registry.createEntity();

	for (auto const& type : CopiedEntity)
	{
		rttr::type obj_type = rttr::type::get_by_name(type.get_type().get_name().to_string());

		// Create component
		Componentregistry.createComponent(type.get_type().get_name().to_string(), ent);

		auto comp = registry.getComponentsRTTR(ent, obj_type);
		auto properties = obj_type.get_properties();

		if (properties.empty()) {
			continue; // Skip empty components
		}

		for (auto& prop : properties)
		{
			const std::string prop_name = prop.get_name().to_string();

			// Find the property in the copied component
			rttr::property copiedProp = obj_type.get_property(prop_name);
			if (!copiedProp.is_valid()) {
				std::cerr << "Unknown property: " << prop_name << std::endl;
				continue;
			}

			// Set the property value based on its type
			auto prop_value = copiedProp.get_value(type);
			if (!prop_value.is_valid()) continue;

			if (prop.get_type() == rttr::type::get<int>()) {
				prop.set_value(comp, prop_value.to_int());
			}
			else if (prop.get_type() == rttr::type::get<float>()) {
				prop.set_value(comp, prop_value.to_float());
			}
			else if (prop.get_type() == rttr::type::get<std::string>()) {
				prop.set_value(comp, prop_value.to_string());
			}
			else if (prop.get_type() == rttr::type::get<bool>()) {
				prop.set_value(comp, prop_value.to_bool());
			}
			else if (prop.get_type() == rttr::type::get<glm::vec3>()) {
				prop.set_value(comp, prop_value.get_value<glm::vec3>());
			}
			else if (prop.get_type() == rttr::type::get<EntityRegistry::Entity>()) {
				prop.set_value(comp, prop_value.get_value<EntityRegistry::Entity>());
			}
			else {
				std::cerr << "Unsupported property type: " << prop_name << std::endl;
			}
		}
	}
}



void selectEntities(std::vector<EntityRegistry::Entity> entities)
{
	for (auto entity : entities)
	{
		auto selectComponent = registry.getComponent<selectionComponent>(entity);

		if (ImGui::Selectable(entityNamePanel(entity).c_str(), selectComponent->selected))
		{

			selectEntity(entities);


			selectComponent->selected = !selectComponent->selected;

		}

		
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C,false))
		{
			if (selectComponent->selected)
			{
				copyEntity(entity);
				std::cout << "copied" << std::endl;
			}
		}
		dragDropEntitySource(entity);

	}

}

void entityPanelUpdate()
{
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_V, false))
	{
		pasteEntity();
	}
}


void entityPanelRender()
{
	ImGui::Begin("Entities");

	auto entitywithselectComponent = registry.getEntitiesWithComponent<selectionComponent>();

	selectEntities(entitywithselectComponent);

	addEntitiesButton();
	ImGui::End();

}


/**************************************************************************************************/


// Scene Panel
/**************************************************************************************************/

unsigned int FBO;
unsigned int RBO;
unsigned int texture_id;
unsigned int pickingTexture;

ImVec2 previousSize{ ScreenWidth,Screenheight };

SceneManager sceneManager;

ImGuizmo::OPERATION ImGuiMode = ImGuizmo::TRANSLATE;

void createFramebuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//for normal texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(ScreenWidth), static_cast<GLsizei>(Screenheight), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);


	//for object picking
	glGenTextures(1, &pickingTexture);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(ScreenWidth), static_cast<GLsizei>(Screenheight), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pickingTexture, 0);


	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(ScreenWidth), static_cast<GLsizei>(Screenheight));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


// and we rescale the buffer, so we're able to resize the window
void rescaleFramebuffer(GLsizei width, GLsizei height)
{


		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

		glBindTexture(GL_TEXTURE_2D, pickingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pickingTexture, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

}


void initializeScenePanel()
{
	createFramebuffer();
}


void updateScenePanel()
{
	if (glfwGetKey(globalWindow, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(globalWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		std::cout << ScenePlaying << "\n";

		sceneManager.SaveScene(ScenePlaying.c_str());

	}
}

void setScenePanelSize(GLsizei width, GLsizei Height)
{


	Screenheight = static_cast<float>(Height);
	ScreenWidth = static_cast<float>(width);
	// we rescale the framebuffer to the actual window size here and reset the glViewport
 
	rescaleFramebuffer(width, Height);
	glViewport(0, 0, width, Height);


}


void displayImagesOnScreen(GLsizei width, GLsizei height)
{
	ImVec2 uv_min(0.0f, 1.0f); // (0, 1) for the top-left corner
	ImVec2 uv_max(1.0f, 0.0f); // (1, 0) for the bottom-right corner

	ImGui::Image((void*)(intptr_t)texture_id, ImVec2((float)width, (float)height),uv_min,uv_max);
}

void loadAndPlayScene(const char* path)
{
	registry.restartEntityCount();
	sceneManager.LoadScene(path);
	ScenePlaying = path;
	std::cout << "Now Playing Scene: " << ScenePlaying << std::endl;
}


void handleDragDropScene(float width, float Height)
{
	ImVec2 imageMin = ImGui::GetItemRectMin();  // The bottom-left corner of the image
	ImVec2 imageMax = ImGui::GetItemRectMax();  // The top-right corner of the image

	if (ImGui::BeginDragDropTarget()) {
		if (ImGui::IsMouseHoveringRect(imageMin, imageMax, false))  // Check if the mouse is over the image
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const char* path = static_cast<const char*>(payload->Data);
				if (path) {
					loadAndPlayScene(path);
				}
			}
		}
		ImGui::EndDragDropTarget();

	}

}


//void moveEntity(EntityRegistry::Entity ent)
//{
//	static ImVec2 oldPositions = { 0,0 };
//
//
//
//	ImVec2 mousePos = ImGui::GetMousePos();
//	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
//		oldPositions = mousePos;
//		return;
//	}
//
//	auto TransformComp = registry.getComponent<TransformComponent>(ent);
//	if (!TransformComp) return;
//	if (!ImGui::IsWindowFocused()) return;
//
//
//	if (oldPositions.x == 0 && oldPositions.y == 0) {
//		oldPositions = mousePos;
//	}
//
//	TransformComp->translate += glm::vec3(mousePos.x - oldPositions.x, -(mousePos.y - oldPositions.y), 0);
//
//	oldPositions = mousePos; // Update for next frame
//
//}

void moveSelectedEntity()
{
	auto entities = registry.getEntitiesWithComponent<selectionComponent>();
	for (auto ent : entities)
	{
		auto selectionComp = registry.getComponent<selectionComponent>(ent);
		if (selectionComp->selected)
		{
			ImGuizmo::BeginFrame();
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			//float windowWidth = (float)ImGui::GetWindowWidth();
			//float windowHeight = (float)ImGui::GetWindowHeight();

			ImVec2 viewportMin = ImGui::GetWindowContentRegionMin();
			ImVec2 viewportMax = ImGui::GetWindowContentRegionMax();
			ImVec2 viewportSize = ImVec2(viewportMax.x - viewportMin.x, viewportMax.y - viewportMin.y);

			ImVec2 minBound = ImGui::GetWindowPos();
			minBound.x += viewportMin.x;
			minBound.y += viewportMin.y;


			//std::cout << "minX: " << minBound.x << "minY: " << minBound.y << std::endl;
			//std::cout << "sizeX: " << viewportSize.x << "sizeY: " << viewportSize.y << std::endl;
			ImGuizmo::SetRect(minBound.x, minBound.y, viewportSize.x, viewportSize.y);


			glm::mat4 cameraView = cam.getviewmatrix();
			glm::mat4 cameraProjection = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);
			
			auto transformComp = registry.getComponent<TransformComponent>(ent);
			glm::mat4 transformMatrix = transformComp->transform;

			if (GetKeyTriggered(GLFW_KEY_Q))
			{
				ImGuiMode = ImGuizmo::TRANSLATE;
			}
			if (GetKeyTriggered(GLFW_KEY_W))
			{
				ImGuiMode = ImGuizmo::ROTATE;
			}
			if (GetKeyTriggered(GLFW_KEY_E))
			{
				ImGuiMode = ImGuizmo::SCALE;
			}

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), ImGuiMode, ImGuizmo::WORLD, glm::value_ptr(transformMatrix));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translate, scale, skew;
				glm::quat rotate;
				glm::vec4 perspective;

				glm::decompose(transformMatrix, scale, rotate, translate, skew, perspective);

				transformComp->translate = translate;
				//transformComp->rotation = ;
				transformComp->scale = scale;

			}
			//moveEntity(ent);

			
		}
	}
}



void entityPicking()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (!ImGui::IsWindowFocused()) return;
		if (ImGuizmo::IsUsing()) return;

		ImVec2 viewportMin = ImGui::GetWindowContentRegionMin();
		ImVec2 viewportMax = ImGui::GetWindowContentRegionMax();
		ImVec2 viewportSize = ImVec2(viewportMax.x - viewportMin.x, viewportMax.y - viewportMin.y);

		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportMin.x;
		minBound.y += viewportMin.y;

		ImVec2 maxbound = ImGui::GetWindowPos();
		maxbound.x += viewportMax.x;
		maxbound.y += viewportMax.y;

		ImVec2 mousePos = ImGui::GetMousePos();

		int x = static_cast<int>(mousePos.x - minBound.x);
		int y = static_cast<int>(maxbound.y - mousePos.y);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glReadBuffer(GL_COLOR_ATTACHMENT1); // Read from the picking texture


		glFlush(); // Ensure all rendering is done before reading

		unsigned char pixel[3];
		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

		EntityRegistry::Entity objectID = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16); // Assuming ID is stored in the red channel

		selectEntity(registry.getEntitiesWithComponent<selectionComponent>());
		setEntitySelectFlag(objectID, true);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind to restore default framebuffer
	}

}
void renderScenePanel(std::string SceneName)
{
	if (debug)
	{



		
		ImGui::Begin(SceneName.c_str());

			if (isPaused)
			{
				if (ImGui::Button("Pause"))
				{
					isPlaying = true;
					isPaused = false;
				}
			}
			else
			{
				if (ImGui::Button("Play"))
				{
					isPlaying = true;
					isPaused = true;

				}
			}
		
		ImGui::SameLine();

		if (ImGui::Button("Stop"))
		{
			isPlaying = false;
			isPaused = false;
		}

		float window_width = ImGui::GetContentRegionAvail().x;
		float window_height = ImGui::GetContentRegionAvail().y;

		ImVec2 currentSize = ImGui::GetWindowSize();

		if (previousSize.x != currentSize.x || previousSize.y != currentSize.y) {

			setScenePanelSize(static_cast<GLsizei>(window_width), static_cast<GLsizei>(window_height));
		}
		previousSize = currentSize;
		// we get the screen position of the window
		displayImagesOnScreen(static_cast<GLsizei>(window_width), static_cast<GLsizei>(window_height));
		handleDragDropScene(window_width, window_height);
		moveSelectedEntity();

		entityPicking();

		cam.processMovement();

		ImGui::End();

	}

}


// here we bind our framebuffer
void bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

}

// here we unbind our framebuffer
void unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
/**************************************************************************************************/


// Content Panel
/**************************************************************************************************/

constexpr char AssetsDirectory[] = "Assets";
static std::filesystem::path m_currentDirectory = AssetsDirectory;


void renderNavigationButton() {
	if (m_currentDirectory.string() != AssetsDirectory) {
		if (ImGui::Button("<-")) {
			m_currentDirectory = m_currentDirectory.parent_path();
		}
	}
}


void renderDragDropSource(const std::string& path, const std::string& filename) {
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
		ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", path.c_str(), path.size() + 1);
		ImGui::Text("Dragging: %s", filename.c_str());
		ImGui::EndDragDropSource();
	}
}


void renderDirectoryContents() {
	for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory)) {
		std::string path = entry.path().string();
		std::string filename = entry.path().filename().string();

		if (ImGui::Button(filename.c_str())) {
			if (entry.is_directory()) {
				m_currentDirectory /= entry.path().filename();
			}
		}

		renderDragDropSource(path, filename);
	}
}


void renderContentPanel() {
	ImGui::Begin("Content");

	renderNavigationButton();
	renderDirectoryContents();

	ImGui::End();
}

/**************************************************************************************************/


// ImGui Settings
/**************************************************************************************************/
void configureImGuiIO(ImGuiIO& io) {
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
}

void setupImGuiWindowStyle(ImGuiIO& io) {
	//ImGui::StyleColorsDark();
	ImGuiStyle& mStyle = ImGui::GetStyle();

	mStyle.WindowMinSize = ImVec2(160, 20);
	mStyle.FramePadding = ImVec2(4, 2);
	mStyle.ItemSpacing = ImVec2(6, 2);
	mStyle.ItemInnerSpacing = ImVec2(6, 4);
	mStyle.Alpha = 0.95f;
	mStyle.WindowRounding = 4.0f;
	mStyle.FrameRounding = 2.0f;
	mStyle.IndentSpacing = 6.0f;
	mStyle.ItemInnerSpacing = ImVec2(2, 4);
	mStyle.ColumnsMinSpacing = 50.0f;
	mStyle.GrabMinSize = 14.0f;
	mStyle.GrabRounding = 16.0f;
	mStyle.ScrollbarSize = 12.0f;
	mStyle.ScrollbarRounding = 16.0f;

	ImGuiStyle& style = mStyle;
	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);


}
/**************************************************************************************************/



// ImGui Initialization
/**************************************************************************************************/
void initializeImGui(GLFWwindow* window)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();


	configureImGuiIO(io);

	setupImGuiWindowStyle(io);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	initializeScenePanel();

	meshRenderInitialisation();

	auto entity = registry.getEntitiesWithComponent<textComponent>();
	for (auto ent : entity)
	{
		fontInitialise("Assets/liberation-mono.ttf", ent);
	}
}
/**************************************************************************************************/

// ImGui Update
/**************************************************************************************************/
void updateImGui()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DeltaTime = static_cast<float>(WejseGetDT());



	inspectorPanelUpdate();
	entityPanelUpdate();
	if (GetKeyTriggered(GLFW_KEY_F))
	{
		auto entity = registry.getEntitiesWithComponent<selectionComponent>();
		for (auto& ent : entity) 
		{
			auto slection = registry.getComponent<selectionComponent>(ent);
			if (slection->selected)
			{

				cam.FindEntity(registry, ent);
			}
		}
	}
	updateScenePanel();

	// Start ImGui frame

	ImGui_ImplGlfw_NewFrame();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}
/**************************************************************************************************/

// ImGui Render
/**************************************************************************************************/
void setImGUISize(ImGuiIO& io, float localScreenWidth, float localScreenheight)
{
	io.DisplaySize = ImVec2(localScreenWidth, localScreenheight);

}

void backUpContext()
{
	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup_current_context);
}

void renderImGui()
{


	ImGuiIO& io = ImGui::GetIO();
	setImGUISize(io, ScreenWidth, Screenheight);

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if (debug)
	{

		bindFramebuffer(); // Comment this out if you are rendering to the default framebuffer

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
		UpdateTransform(registry);

		// 1. Render opaque objects (no blending)

		meshRenderRender();  // Render opaque meshes

		renderUpdate(registry);  // Update the transparent objects
		fontRenderer();

		// Unbind the framebuffer
		unbindFramebuffer(); // Comment this out if rendering to the default framebuffer

	}

	ImVec2 defaultSize(800, 600); // Valid non-zero size
	ImGui::SetNextWindowSize(defaultSize, ImGuiCond_Always);
	renderContentPanel();

	ImGui::SetNextWindowSize(defaultSize, ImGuiCond_Always);
	entityPanelRender();

	ImGui::SetNextWindowSize(defaultSize, ImGuiCond_Always);
	inspectorPanelRender();

	ImGui::SetNextWindowSize(defaultSize, ImGuiCond_Always);
	renderScenePanel("Viewport");


	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		backUpContext();
	}

}
/**************************************************************************************************/


// ImGui CleanUp
/**************************************************************************************************/
void cleanupImGui()
{
	fontCleanUp();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
/**************************************************************************************************/

