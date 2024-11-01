#include "Game.h"
#include <iostream>
#include "BlocksGame.h"
#include <thread>
#include "Input.h"
#include "Renderer.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
BlocksGame game;
int Game::windowWidth = 1920;
int Game::windowHeight = 1080;

Game::Game()
{
	if (!glfwInit()) {
		std::cout << "glfw failed to init";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "BlocksGame", NULL, NULL);
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
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL error after call: " << err << std::endl;
	}

	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);

	Renderer renderer = Renderer();

	game = BlocksGame();
	std::thread BlocksGameThread(&BlocksGame::tick, &game);


	glViewport(0, 0, windowWidth, windowHeight);
	while (!glfwWindowShouldClose(window)) {

		update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer.draw();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	BlocksGameThread.join();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	Game::windowWidth = width;
	Game::windowHeight = height;
	glViewport(0, 0, width, height);

}

void Game::update()
{
	game.update();

}


