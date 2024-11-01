#define STB_IMAGE_IMPLEMENTATION
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb/stb_image.h"'
#include "BlocksGame.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"
#include <filesystem>

Renderer::Renderer()
{

	loadMesh();
	createShaders();
	loadTextures();

}

void Renderer::loadMesh()
{

	// buffer creation
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// buffer vertex data to gpu
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// buffer indices data to the gpu
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);



}


void Renderer::loadTextures()
{


	std::vector<std::string> pathList;

	for (const auto& file : std::filesystem::directory_iterator("Resources/Textures/Block/")) {
		pathList.push_back(file.path().string().c_str());
	}


	// stbi image loading
	stbi_set_flip_vertically_on_load(true);
	for (const auto& file : pathList) {

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
		unsigned char* data = stbi_load(file.c_str(), &width, &height, &channels, 0);

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
		textureIDs.push_back(texture);
	}

}

void Renderer::createShaders()
{
	
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	// code as string
	std::string sVertexShaderCode;
	std::string sFragmentShaderCode;

	vertexShaderFile.open("default_vertex.glsl");
	fragmentShaderFile.open("default_fragment.glsl");

	// stream object
	std::stringstream vertexStream, fragmentStream;

	// read content into stream;
	vertexStream << vertexShaderFile.rdbuf();
	fragmentStream << fragmentShaderFile.rdbuf();

	// close files
	vertexShaderFile.close();
	fragmentShaderFile.close();

	// convert to string
	sVertexShaderCode = vertexStream.str();
	sFragmentShaderCode = fragmentStream.str();

	const char* vertexCode;
	const char* fragmentCode;

	// convert to const char*
	vertexCode = sVertexShaderCode.c_str();
	fragmentCode = sFragmentShaderCode.c_str();



	unsigned int vertexShader;
	unsigned int fragmentShader;
	// create our shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// specify the shader source
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);

	// compile shaders
	glCompileShader(vertexShader);

	int success;
	char info[512];

	// error checking
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(vertexShader, 512, NULL, info);
	if (!success) {
		std::cout << "Vertex´shader compilation failed" << info << std::endl;
	}

	glCompileShader(fragmentShader);

	// error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(fragmentShader, 512, NULL, info);
	if (!success) {
		std::cout << "Fragment shader compilation failed" << info << std::endl;
	}

	// create the shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// delete the shaders as we have the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Renderer::draw()
{

	glBindVertexArray(VAO);
	glUseProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture);

	// grab variables from the other classes
	float gameWidth = static_cast<float>(BlocksGame::width);
	float gameHeight = static_cast<float>(BlocksGame::height);
	float windowWidth = static_cast<float>(Game::windowWidth);
	float windowHeight = static_cast<float>(Game::windowHeight);

	// calculate aspect ratio of window and game size
	float windowAspectRatio = windowWidth / windowHeight;
	float gameAspectRatio = gameWidth / gameHeight;
	float left, right, bottom, top;

	if (windowAspectRatio > gameAspectRatio) {
		// Window is wider than game area
		float adjustedWidth = gameHeight * windowAspectRatio;
		left = -adjustedWidth / 2.0f;
		right = adjustedWidth / 2.0f;

		bottom = -gameHeight / 2.0f;
		top = gameHeight / 2.0f;
	}
	else {
		// Window is taller than game area
		float adjustedHeight = gameWidth / windowAspectRatio;
		left = -gameWidth / 2.0f;
		right = gameWidth / 2.0f;

		bottom = -adjustedHeight / 2.0f;
		top = adjustedHeight / 2.0f;
	}

	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::ortho(left, right, bottom, top);


	mutex.lock(); // to prevent possible race condition but I dont believe its essential.
	for (int x = 0; x < BlocksGame::wholeMapShape.width; x++)
	{
		for (int y = 0; y < BlocksGame::wholeMapShape.height; y++)
		{

			char letter = BlocksGame::wholeMapShape.shape[x + y * BlocksGame::width];
			if (letter != '*') {

				view = glm::mat4(1.0f);
				// we add 0.5f or subtract because our shapes are got size of -0.5f to 0.5f
				pos = glm::vec3(x+0.5f -gameWidth/2, BlocksGame::wholeMapShape.height - y-0.5 -gameHeight/2, 0);
				view = glm::translate(view, pos);

				// update shader unifom with view matrix
				unsigned int transformLoc = glGetUniformLocation(shaderProgram, "view");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));

				// update shader unifom with proj matrix
				unsigned int projLoc = glGetUniformLocation(shaderProgram, "proj");
				glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

				glActiveTexture(GL_TEXTURE0);
				// transform a char that we ASSUME is a valid digit
				int digit = letter - '0'; 
				glBindTexture(GL_TEXTURE_2D, textureIDs[digit]); // bind relevant texture with this digit
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}

		}
	}
	mutex.unlock();

}
