#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>

class texture
{
private :
	unsigned int ID;
	bool hastexture;
public:
	texture(const char* filePath) 
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;

		unsigned char* data = stbi_load(filePath, & width, & height, & nrChannels, 0);
		if (data)
		{
			hastexture = true;



			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);



			stbi_set_flip_vertically_on_load(true);
		}
		else
		{
			hastexture = false;
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	~texture()
	{
		glDeleteTextures(1, &ID);
	}

	void use()
	{
		glBindTexture(GL_TEXTURE_2D, ID);

	}

	bool isValid()
	{
		return hastexture;
	}
};