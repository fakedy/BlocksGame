#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>


class Game
{
public:
	Game();

private:
	void update();


	GLFWwindow* window;

};

