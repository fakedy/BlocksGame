#include "Game.h"
#include <iostream>
#include "BlocksGame.h"
#include <thread>
#include "Input.h"
#include "Renderer.h"


BlocksGame game;

Game::Game()
{
	if (!glfwInit()) {
		std::cout << "glfw failed to init";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "BlocksGame", NULL, NULL);
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

	Renderer renderer = Renderer();

	game = BlocksGame();
	std::thread BlocksGameThread(&BlocksGame::tick, &game);


	glViewport(0, 0, 1920, 1080);
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

void Game::update()
{
	game.update();

}
