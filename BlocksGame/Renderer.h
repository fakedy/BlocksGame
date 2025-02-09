#pragma once
#include <glad/glad.h>
#include "BlocksGame.h"
#include <mutex>
#include "TextureManager.h"
class Renderer
{

public:
	Renderer();

	void draw();

private:

	TextureManager textureManager;
	unsigned int VBO{};
	unsigned int VAO{};
	unsigned int EBO{};
	unsigned int shaderProgram{};

	unsigned int backgroundBlockTexture{};
	unsigned int markerBlockTexture{};
	std::vector<GLuint> textureIDs;

	float vertices[16] = {
		// Position       // Texture Coordinates (optional)
		-0.5f, -0.5f,     0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f,     1.0f, 0.0f,  // Bottom-right
		 0.5f,  0.5f,     1.0f, 1.0f,  // Top-right
		-0.5f,  0.5f,     0.0f, 1.0f   // Top-left
	};
	unsigned int indices[6] = {
	0, 1, 2,   // First triangle (bottom-left, bottom-right, top-right)
	2, 3, 0    // Second triangle (top-right, top-left, bottom-left)
	};

	void loadMesh();
	void loadTextures();
	void createShaders();

};

