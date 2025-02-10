#include "Game.h"
#include <iostream>
#include "BlocksGame.h"
#include "Input.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int Game::windowWidth{ 1920 };
int Game::windowHeight{ 1080 };


Game::Game()
{
	if (!glfwInit()) {
		std::cout << "glfw failed to init";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(windowWidth, windowHeight, "BlocksGame", NULL, NULL);
	if (!window) {
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}

	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);


	// sound stuff

	
	ma_result result;
	ma_engine soundEngine;

	result = ma_engine_init(NULL, &soundEngine);
	if (result != MA_SUCCESS) {
		printf("Failed to initialize audio engine.\n");
		return;
	}
	


	renderer = std::make_unique<Renderer>();

	game = std::make_unique<BlocksGame>(soundEngine);


	glViewport(0, 0, windowWidth, windowHeight);
	while (!glfwWindowShouldClose(window)) {
		update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer->draw();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	game->stop();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	Game::windowWidth = width;
	Game::windowHeight = height;
	glViewport(0, 0, width, height);

}

void Game::update()
{
	game->tick();
	game->update();

}


