#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "SOIL/SOIL.h"
#include <iostream>

class texture
{
private :
	unsigned int ID;
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
		GLuint tex_ID = SOIL_load_OGL_texture(
			filePath,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_POWER_OF_TWO
			| SOIL_FLAG_MIPMAPS
			//| SOIL_FLAG_MULTIPLY_ALPHA
			//| SOIL_FLAG_COMPRESS_TO_DXT
			| SOIL_FLAG_DDS_LOAD_DIRECT
			//| SOIL_FLAG_NTSC_SAFE_RGB
			//| SOIL_FLAG_CoCg_Y
			//| SOIL_FLAG_TEXTURE_RECTANGLE
		);
		if (tex_ID > 0)
		{
			//	enable texturing
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glEnable( 0x84F5 );// enables texture rectangle
			//  bind an OpenGL texture ID
			ID = tex_ID;
			//	report
			std::cout << "the loaded texture ID was " << tex_ID << std::endl;
			//std::cout << "the load time was " << 0.001f * time_me << " seconds (warning: low resolution timer)" << std::endl;
		}
		else
		{
			//	loading of the texture failed...why?
			glDisable(GL_TEXTURE_2D);
			std::cout << "Texture loading failed: '" << SOIL_last_result() << "'" << std::endl;
		}
		//stbi_image_free(data);
	}

	~texture()
	{
		glDeleteTextures(1, &ID);
	}

	void use()
	{
		glBindTexture(GL_TEXTURE_2D, ID);

	}
};