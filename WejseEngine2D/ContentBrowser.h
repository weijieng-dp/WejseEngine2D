#pragma once
#include "imgui.h"
#include "texture_s.h"
#include <filesystem>

class ContentBroswer
{
public:
	void initialise()
	{

	}

	void render() {
		ImGui::Begin("Content");

		if (m_currentDirectory.string() != s_AssetsDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}


		for (auto& p : std::filesystem::directory_iterator(m_currentDirectory))
		{
			std::string path = p.path().string();
			auto relativepath = std::filesystem::relative(p.path(), s_AssetsDirectory);
			std::string filenamestring = relativepath.filename().string();

			if (ImGui::Button(filenamestring.c_str()))
			{
				if (p.is_directory())
					m_currentDirectory /= p.path().filename();
			}

			// Set up drag source
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", path.c_str(), path.size() + 1);
				ImGui::Text("Dragging: %s", filenamestring.c_str());
				ImGui::EndDragDropSource();
			}
		}
		ImGui::End();
	}

	void cleanUp()
	{

	}
private:
	std::filesystem::path m_currentDirectory = "Assets";
	std::string s_AssetsDirectory = "Assets";
	
};