#pragma once
#include <glad/glad.h>
#include "BlocksGame.h"
class Renderer
{

public:
	Renderer();

	void draw();

private:

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	float vertices[16] = {
		// Position       // Texture Coordinates (optional)
		-1.0f, -1.0f,     0.0f, 0.0f,  // Bottom-left
		 1.0f, -1.0f,     1.0f, 0.0f,  // Bottom-right
		 1.0f,  1.0f,     1.0f, 1.0f,  // Top-right
		-1.0f,  1.0f,     0.0f, 1.0f   // Top-left
	};
	unsigned int indices[6] = {
	0, 1, 2,   // First triangle (bottom-left, bottom-right, top-right)
	2, 3, 0    // Second triangle (top-right, top-left, bottom-left)
	};

	void loadMesh();
	void loadTextures();
	void createShaders();

};

