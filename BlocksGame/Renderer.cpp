#include "Renderer.h"
#include <iostream>
#include "BlocksGame.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <sstream>

Renderer::Renderer()
{
	loadMesh();
	createShaders();
	loadTextures();
	textureManager = TextureManager();

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

	// vector of paths
	std::vector<std::string> pathList;
	
	// loop through all files in a certain path and add them to a vector
	for (const auto& file : std::filesystem::directory_iterator("Resources/Textures/Block/PieceBlocks/")) {
		pathList.push_back(file.path().string().c_str());
	}

	// create a texture for every file in the folder and add them to a vector of ID's
	for (const auto& file : pathList) {
		unsigned int texture = textureManager.loadTexture(file.c_str());
		textureIDs.push_back(texture);
	}

	// specific texture loading for background and marker
	backgroundBlockTexture = textureManager.loadTexture("Resources/Textures/Block/blackBlock.png");
	markerBlockTexture = textureManager.loadTexture("Resources/Textures/Block/markerBlock.png");

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


	for (int x = 0; x < BlocksGame::wholeMapShape.width; x++)
	{
		for (int y = 0; y < BlocksGame::wholeMapShape.height; y++)
		{

			char letter = BlocksGame::wholeMapShape.shape[x + y * BlocksGame::width];

				view = glm::mat4(1.0f);
				// we add 0.5f or subtract because our shapes are got size of -0.5f to 0.5f
				pos = glm::vec3(x+0.5f -gameWidth/2, BlocksGame::wholeMapShape.height - y -0.5 -gameHeight/2, 0);
				view = glm::translate(view, pos);

				// update shader unifom with view matrix
				unsigned int transformLoc = glGetUniformLocation(shaderProgram, "view");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));

				// update shader unifom with proj matrix
				unsigned int projLoc = glGetUniformLocation(shaderProgram, "proj");
				glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

			// if the block is not empty
			if (letter != '*') {
				glActiveTexture(GL_TEXTURE0);

				// if the block is a marker block
				if (letter == 'x') {
					glBindTexture(GL_TEXTURE_2D, markerBlockTexture); 
				}
				else { // if it is a normal block
					// transform a char that we ASSUME is a valid digit
					int digit = letter - '0'; 
					glBindTexture(GL_TEXTURE_2D, textureIDs[digit]); // bind relevant texture with this digit
				}

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			else { // draw an empty background block
				
				glActiveTexture(GL_TEXTURE0);
				// transform a char that we ASSUME is a valid digit
				int digit = letter - '0';
				glBindTexture(GL_TEXTURE_2D, backgroundBlockTexture); // bind relevant texture with this digit
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				
			}

		}
	}

}
