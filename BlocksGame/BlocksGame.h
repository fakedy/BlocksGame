#pragma once
#include <array>
#include <span>
#include <vector>
#include "Tshape.h"
#include "Ishape.h"

class BlocksGame
{






public:

	static const int width = 10;
	static const int height = 20;
	bool hasmoved;
	Shape* piece;

	constexpr std::vector<char> fillMap();

	BlocksGame();

	void tick();
	void update();

	Shape rotate(Shape shape);

	Shape transpose(Shape shape);
	Shape reverse(Shape shape);


	bool canMove(const Shape& map, const Shape piece);

	void move(Shape& shape, int x, int y);

	Shape stitch(const Shape& shape1, const Shape& shape2);

	void printMap(Shape shape);

};

