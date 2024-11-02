#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>


class Game
{
public:
	Game();

	static int windowWidth;
	static int windowHeight;

private:
	void update();



	GLFWwindow* window{};

};

