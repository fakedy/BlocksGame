#include "Game.h"
#include<iostream>
#include "BlocksGame.h"
#include <thread>



BlocksGame game;



Game::Game()
{
	if (!glfwInit()) {
		std::cout << "glfw failed to init";
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "BlocksGame", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	game = BlocksGame();
	std::thread BlocksGameThread(&BlocksGame::tick, &game);

	glViewport(0, 0, 1920, 1080);
	while (!glfwWindowShouldClose(window)) {

		update();

		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

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
