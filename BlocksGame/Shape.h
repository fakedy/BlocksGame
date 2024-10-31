#pragma once
#include <vector>


class Shape {

public:

	int width;
	int height;
	int posX;
	int posY;

	void transpose();
	void reverse();
	void rotate();
	std::vector<char> shape;




};