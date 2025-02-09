#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include "Renderer.h"
#include <memory>

class Game
{
public:
	Game();

	static int windowWidth;
	static int windowHeight;

private:
	void update();


	std::unique_ptr<BlocksGame> game;
	GLFWwindow* window;
	std::unique_ptr<Renderer> renderer;

};

