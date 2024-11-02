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




};