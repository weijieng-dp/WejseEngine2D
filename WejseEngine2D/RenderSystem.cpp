
#include "RenderSystem.h"

static camera& cam = camera::instance();
static Registry& registry = Registry::instance();

static GLuint QuadVAO, QuadVBO, QuadEBO;
static GLuint TriVAO, TriVBO, TriEBO;
static GLuint LineVAO, LineVBO;
static GLuint circleVAO, circleVBO;



void initialiseQuad()
{
	// 4 vertices: (x, y, z) and (u, v)
	float vertices[] = {
		// positions         // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f   // top left 
	};

	unsigned int indices[] = {
	0, 1, 3,   // first triangle (top right, bottom right, top left)
	1, 2, 3    // second triangle (bottom right, bottom left, top left)
	};

	glGenBuffers(1, &QuadVBO);
	glGenVertexArrays(1, &QuadVAO);
	glGenBuffers(1, &QuadEBO);

	glBindVertexArray(QuadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Set up vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up texture coordinates attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void initialiseTriangle()
{

	float vertices[] = {
		// positions         
		 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f

	};


	glGenBuffers(1, &TriVBO);
	glGenVertexArrays(1, &TriVAO);

	glBindVertexArray(TriVAO);

	glBindBuffer(GL_ARRAY_BUFFER, TriVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
}




void initialiseLines()
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

void initialiseCircle()
{
	std::vector<float> vertices;

	// Add the center vertex
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	// Add the circumference vertices
	for (int i = 0; i <= 32; ++i) {
		float angle = (2.0f * static_cast<float>(M_PI) * i) / 32;
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

void initialiseShapes()
{
	initialiseCircle();
	initialiseLines();
	initialiseQuad();
	initialiseTriangle();
}



void drawCircle() {
	glBindVertexArray(circleVAO);
	glDrawArrays(GL_LINE_LOOP, 1, 32);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawfilledCircle() {


	// Draw the circle
	glBindVertexArray(circleVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 34);


}

void DrawQuads()
{
	glBindVertexArray(QuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawTriangle()
{
	glBindVertexArray(TriVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Only two vertices for a line

}

void drawLine() {
	glBindVertexArray(LineVAO);
	glDrawArrays(GL_LINES, 0, 2); // Only two vertices for a line
}


void RenderCleanUp()
{
	glDeleteVertexArrays(1, &QuadVAO);
	glDeleteBuffers(1, &QuadVBO);
	glDeleteBuffers(1, &QuadEBO);
	glDeleteVertexArrays(1, &TriVAO);
	glDeleteBuffers(1, &TriVBO);
	glDeleteBuffers(1, &TriEBO);
	glDeleteVertexArrays(1, &LineVAO);
	glDeleteBuffers(1, &LineVBO);
	glDeleteVertexArrays(1, &circleVAO);
	glDeleteBuffers(1, &circleVBO);
}

void updateTexture(const std::string& newTexturePath, Registry::Entity entity) {
	auto rendercomp = registry.getComponent<SpriteRenderComponent>(entity);

	auto tex = std::make_unique<texture>((newTexturePath).c_str()); // Load the new texture

	if (!tex->isValid())
	{
		rendercomp->hasTexture = false;
	}
	else
	{
		rendercomp->hasTexture = true;
		rendercomp->tex = std::make_unique<texture>((newTexturePath).c_str()); // Load the new texture
		rendercomp->TextureString = newTexturePath;
	}
}


void renderInitialise()
{
	initialiseQuad();
}

void meshRenderInitialisation()
{
	initialiseCircle();
	initialiseTriangle();
	initialiseLines();
}



void renderUpdate(Registry& registries)
{

	glm::mat4 viewMatrix;
	if (debug)
	{
		viewMatrix = cam.getviewmatrix();

	}
	else
	{
		auto Cameraent = registries.getEntitiesWithComponent<CameraComponent>();
		auto transformcomp = registries.getComponent<TransformComponent>(Cameraent[0]);

		viewMatrix = glm::translate(glm::mat4(1), -transformcomp->translate);

	}
	glm::mat4 projectionMatrix = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);
	// Retrieve all entities with a SpriteRenderComponent
	auto entitiesWithRender = registries.getEntitiesWithComponent<SpriteRenderComponent>();

	//sortlayer(entitiesWithlayer, registry);



	for (auto& entity : entitiesWithRender) {
		auto* renderComp = registries.getComponent<SpriteRenderComponent>(entity);
		auto* transformComp = registries.getComponent<TransformComponent>(entity);
		auto activeComp = registries.getComponent<ActiveComponent>(entity);

		if (activeComp && activeComp->isActive)
		{


			// Set matrices in the shader
			renderComp->shader->use();
			renderComp->shader->setMat4("model", transformComp->transform);
			renderComp->shader->setMat4("view", viewMatrix);
			renderComp->shader->setMat4("projection", projectionMatrix);

			renderComp->shader->setVec3("uColor", renderComp->color);
			glm::vec3 encodedID = glm::vec3((entity & 0xFF) / 255.0f,
				((entity >> 8) & 0xFF) / 255.0f,
				((entity >> 16) & 0xFF) / 255.0f);
			renderComp->shader->setVec3("encodedID", encodedID);


			renderComp->shader->setBool("hasTexture", true);


			auto lightEnt = registry.getEntitiesWithComponent<lightComponent>();
			int num = 0;

			for (auto ent : lightEnt)
			{

				auto lightTransformComp = registry.getComponent<TransformComponent>(ent);
				glm::vec2 value = glm::vec2(lightTransformComp->translate.x, lightTransformComp->translate.y);
				renderComp->shader->setVec2("lightPosition[" + std::to_string(num) + "]", value);
				renderComp->shader->setFloat("lightIntensity[" + std::to_string(num) + "]", registry.getComponent<lightComponent>(ent)->lightIntensity);
				num++;


			}

			if (num == 0)
			{
				renderComp->shader->setFloat("ambientStrength", 1);
			}
			else
			{
				renderComp->shader->setFloat("ambientStrength", 0);

			}

			renderComp->tex->use();


			DrawQuads();
		}

	}
}


void meshRenderUpdate()
{

}

void meshRenderRender()
{


	glm::mat4 viewMatrix;
	if (debug)
	{
		viewMatrix = cam.getviewmatrix();

	}
	else
	{
		auto Cameraent = registry.getEntitiesWithComponent<CameraComponent>();
		auto transformcomp = registry.getComponent<TransformComponent>(Cameraent[0]);

		viewMatrix = glm::translate(glm::mat4(1), -transformcomp->translate);

	}


	glm::mat4 projectionMatrix = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);







	auto entitywithMeshrendercomp = registry.getEntitiesWithComponent<MeshRenderComponent>();

	for (auto entity : entitywithMeshrendercomp)
	{
		auto meshRenderComp = registry.getComponent<MeshRenderComponent>(entity);
		auto transformComp = registry.getComponent<TransformComponent>(entity);
		auto activeComp = registry.getComponent<ActiveComponent>(entity);

		if (activeComp && activeComp->isActive)
		{
			meshRenderComp->shader->use();

			meshRenderComp->shader->setMat4("model", transformComp->transform);
			meshRenderComp->shader->setMat4("view", viewMatrix);
			meshRenderComp->shader->setMat4("projection", projectionMatrix);

			meshRenderComp->shader->setVec3("uColor", meshRenderComp->color);
			glm::vec3 encodedID = glm::vec3((entity & 0xFF) / 255.0f,
				((entity >> 8) & 0xFF) / 255.0f,
				((entity >> 16) & 0xFF) / 255.0f);
			meshRenderComp->shader->setVec3("encodedID", encodedID);
			meshRenderComp->shader->setBool("hasTexture", false);

			auto lightEnt = registry.getEntitiesWithComponent<lightComponent>();
			int num = 0;

			for (auto ent : lightEnt)
			{
				auto lightTransformComp = registry.getComponent<TransformComponent>(ent);
				glm::vec2 value = glm::vec2(lightTransformComp->translate.x, lightTransformComp->translate.y);
				meshRenderComp->shader->setVec2("lightPosition[" + std::to_string(num) + "]", value);
				meshRenderComp->shader->setFloat("lightIntensity[" + std::to_string(num) + "]", registry.getComponent<lightComponent>(ent)->lightIntensity);
				num++;

			}

			if (num == 0)
			{
				meshRenderComp->shader->setFloat("ambientStrength", 1);
			}
			else
			{
				meshRenderComp->shader->setFloat("ambientStrength", 0);

			}
			if (meshRenderComp->shapeName == "square")
			{
				DrawQuads();
			}
			if (meshRenderComp->shapeName == "triangle")
			{
				DrawTriangle();
			}
			if (meshRenderComp->shapeName == "circle")
			{
				drawfilledCircle();
			}
			if (meshRenderComp->shapeName == "line")
			{
				drawLine();
			}
		}
	}
}

void meshRenderCleanUp()
{

}


//Font Rendering
/************************************************************************************/
/// Holds all state information relevant to a character as loaded using FreeType


unsigned int textVAO, textVBO;

FT_Library ft;


void fontInitialise(std::string fontName, EntityRegistry::Entity ent)
{

	auto textcomp = registry.getComponent<textComponent>(ent);
	textcomp->font = fontName;

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	// find path to font
	std::string font_name = fontName;
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;


			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			textcomp->Characters.insert(std::pair<char, Character>(c, character));
		}

		glBindTexture(GL_TEXTURE_2D, 0);

	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void fontUpdate(std::string fontName, EntityRegistry::Entity ent)
{
	auto textcomp = registry.getComponent<textComponent>(ent);

	if (!textcomp->Characters.empty())
	{
		textcomp->Characters.clear();
	}
	fontInitialise(fontName, ent);

}

void fontRenderer()
{


	auto entitywithtext = registry.getEntitiesWithComponent<textComponent>();

	for (auto entity : entitywithtext)
	{
		auto activeComp = registry.getComponent<ActiveComponent>(entity);
		if (!activeComp->isActive) continue;

		auto textcomp = registry.getComponent<textComponent>(entity);
		auto transformcomp = registry.getComponent<TransformComponent>(entity);

		textcomp->shader->use();
		glUniform3f(glGetUniformLocation(textcomp->shader->ID, "textColor"), textcomp->color.x, textcomp->color.y, textcomp->color.z);
		glm::mat4 projection = glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(textcomp->shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 viewMatrix = glm::mat4(1.0);

		if (debug)
		{
			viewMatrix = cam.getviewmatrix();

		}
		else
		{
			auto Cameraent = registry.getEntitiesWithComponent<CameraComponent>();
			auto transformcompforCam = registry.getComponent<TransformComponent>(Cameraent[0]);

			viewMatrix = glm::translate(glm::mat4(1), -transformcompforCam->translate);

		}


		textcomp->shader->setMat4("view", viewMatrix);

		if (transformcomp)
		{
			textcomp->shader->setMat4("model", transformcomp->transform);
		}


		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textVAO);

		// iterate through all characters
		std::string::const_iterator c;
		std::string text = textcomp->text;
		float x = 0, y = 0;
		float scale = 1;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = textcomp->Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void fontCleanUp()
{
	// Delete character textures
	auto entitywithtext = registry.getEntitiesWithComponent<textComponent>();

	for (auto entity : entitywithtext)
	{

		auto textcomp = registry.getComponent<textComponent>(entity);
		for (auto& [_, ch] : textcomp->Characters) {
			glDeleteTextures(1, &ch.TextureID);
		}
		textcomp->Characters.clear();
	}
	// Delete the vertex buffer and array objects
	if (textVBO != 0) {
		glDeleteBuffers(1, &textVBO);
		textVBO = 0;
	}

	if (textVAO != 0) {
		glDeleteVertexArrays(1, &textVAO);
		textVAO = 0;
	}
}
