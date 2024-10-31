#include "BlocksGame.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include "Tshape.h"
#include "Ishape.h"
#include "Input.h"



std::vector<Shape> shapes;

constexpr std::vector<char> BlocksGame::fillMap() {
	
	std::vector<char> array;
	array.resize(height * width);

	for (int y = 0; y < BlocksGame::height; y++) {

		for (int x = 0; x < BlocksGame::width; x++)
		{
			array[x + y * BlocksGame::width] = '*';
		}

	}

	return array;
}

BlocksGame::BlocksGame() {
	// create the base shape 
	mapShape.shape = fillMap();
	mapShape.width = width;
	mapShape.height = height;

	// add shapes to the list of shapes
	shapes.push_back(Tshape());
	shapes.push_back(Ishape());

	piece = new Shape(shapes[0]);
}

void BlocksGame::tick() {

	while (true) {

		if (piece == nullptr) {
			piece = new Shape(shapes[0]);
		}

		auto now = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

		char buffer[26];  // Buffer to hold the formatted time string
		ctime_s(buffer, sizeof(buffer), &currentTime);
		std::cout << "BlocksGame Clock: " << buffer;


		// if we move
		if (!move(*piece, 0, 1)) {
			// collision
			std::cout << "COLLISION" << std::endl;
			mapShape = stitch(mapShape, *piece);
			delete piece;
			piece = nullptr;
		}


		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


}

void BlocksGame::update() {

	if (piece != nullptr) {
		// detect input and move piece
		if (hasmoved) {
			printMap(stitch(mapShape, *piece));
			hasmoved = false;
		}
		


		if (Input::getKeyPressed(Input::Key::D)) {
			move(*piece, 1, 0);
		}
		else if (Input::getKeyPressed(Input::Key::A)) {
			move(*piece, -1, 0);
		}
		if (Input::getKeyPressed(Input::Key::S)) {
			move(*piece, 0, 1);
		}

		if (Input::getKeyPressed(Input::Key::W)) {
			piece->rotate();
			printMap(stitch(mapShape, *piece));
		}
	}
}

bool BlocksGame::canMove(const Shape& map, const Shape piece) {
	// if we hit bottom
	if (piece.posY+piece.height-1 == height) {
		return false;
	}
	// if we are outside map
	if (piece.posX+piece.width > width) {
		return false;
	}
	// if we are outside map
	if (piece.posX < 0) {
		return false;
	}

	// check overlap
	for (int y = 0; y < piece.height; y++) {
		for (int x = 0; x < piece.width; x++) {
			if (piece.shape[x + y * piece.width] != '*' && map.shape[x + piece.posX + (y + piece.posY) * width] != '*') {
				return false;
			}

		}
	}

	return true;
}

bool BlocksGame::move(Shape& shape, int x, int y) {
	// uh this felt kinda weird to do
	Shape tempPiece = shape;
	tempPiece.posX += x;
	tempPiece.posY += y;

	if (canMove(mapShape, tempPiece)) {
		shape.posX += x;
		shape.posY += y;
		hasmoved = true;
		return true;
	}
	return false;
}

Shape BlocksGame::stitch(const Shape& shape1, const Shape& piece) {
	Shape stitchedShape = shape1; // Start with the base map
	for (int y = 0; y < piece.height; y++) {
		for (int x = 0; x < piece.width; x++) {
			if (piece.shape[x + y * piece.width] != '*') {
				stitchedShape.shape[x+piece.posX + (y + piece.posY) * width] = piece.shape[x + y * piece.width]; // prob edit this
			}
		}
	}
	return stitchedShape;
}

void BlocksGame::printMap(Shape shape) {


	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			std::cout << shape.shape[x + y * width];
		}
		std::cout << std::endl;
	}

}