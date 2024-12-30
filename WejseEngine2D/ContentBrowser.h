#pragma once
#include "imgui.h"
#include "filesystem"

class ContentBroswer
{
public:
	void initialise()
	{

	}

	void render() {
		ImGui::Begin("Content");
		for (auto& p : std::filesystem::directory_iterator("Assets"))
		{
			std::string path = p.path().string();
			if (p.is_directory())
			{
				if (ImGui::Button(path.c_str()))
				{

				}
			}
		}
		ImGui::End();
	}

	void cleanUp()
	{

	}
};