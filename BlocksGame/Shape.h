#pragma once
#include <vector>


class Shape {

public:

	std::vector<char> shape;
	int width{};
	int height{};
	int posX{};
	int posY{};

	void transpose();
	void reverse();
	void rotate();

	Shape(int width, int height, int posX, int posY, std::vector<char> shape) : width(width), height(height), posX(posX), posY(posY) {
		shape;
	}

	Shape() = default;

};