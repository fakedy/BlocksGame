#define STB_IMAGE_IMPLEMENTATION
#include "TextureManager.h"
#include <glad/glad.h>
#include "stb/stb_image.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

int TextureManager::loadTexture(const char* file)
{
	// the textures are upside down if we dont do this
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture;

	// Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, channels;
	unsigned char* data = stbi_load(file, &width, &height, &channels, 0);

	//  defines the textures differently depending on if it got RGB or RGBA aka jpg or png kinda.
	if (data) {
		if (channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture: " << file << stbi_failure_reason() << std::endl;
	}


	stbi_image_free(data);



    return texture;
}
