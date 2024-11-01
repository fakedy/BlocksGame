#include "BlocksGame.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>

#include "Input.h"
#include <random>
#include <functional>



Shape BlocksGame::wholeMapShape;
std::random_device rd; // random seed
std::mt19937 gen(rd()); // init generator with random seed
std::uniform_int_distribution<int> dist(1, 100); // defines our distribution

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


	wholeMapShape.shape = fillMap();
	wholeMapShape.width = width;
	wholeMapShape.height = height;

	// randomly select first piece
	piece = shapeCreators[dist(gen) % shapeCreators.size()]();
}

void BlocksGame::tick() {

	while (true) {

		// if we have no piece, create it.
		if (piece == nullptr) {
			piece = shapeCreators[dist(gen) % shapeCreators.size()]();
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
			wholeMapShape = stitch(mapShape, *piece);
			rowClear(wholeMapShape); // check if we have a full row. gives an int as return (score)
			mapShape = wholeMapShape;
			piece = nullptr;
		}


		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


}

int BlocksGame::rowClear(Shape& map) {

	int counter = 0;
	std::vector<int> locations;
	char prev = '*';

	for (int y = 0; y < map.height; y++) {
		for (int x = 0; x < map.width; x++) {
			
			int index = x + y * map.width;
			if (map.shape[index] != '*') {
				// if we are at start of row or previous one was a block
				if (index % map.width == 0 || prev != '*') {
					counter++;
					
					prev = map.shape[index];
				}

			}
			else {
				// we broke the streak, restart.
				x = map.width;
				counter = 0;
				prev = '*';
			}
			// if we have a row to remove, add it to a list
			if (counter != 0 && counter % map.width == 0) {
				locations.push_back(y);
			}

		}
	}
	// I dont think locking is actually necessary but whatever
	mutex.lock();
	// for every row marked to be removed, remove them and add new row

	// sort rows in descending order to ensure integrity of rows in shape
	std::sort(locations.rbegin(), locations.rend());

	for (int loc : locations)
	{
		auto start = map.shape.begin() + loc * map.width;
		auto end = start + map.width;

		map.shape.erase(start, end);

	}
	
	// inefficient
	// insert new rows from start of vector
	
	for (size_t i = 0; i < locations.size(); i++)
	{
		map.shape.insert(map.shape.begin(), map.width ,'*');
	}
	
	mutex.unlock();

	return counter;

}

void BlocksGame::update() {

	// main thread

	if (piece != nullptr) {
		// detect input and move piece
		if (hasmoved) {
			wholeMapShape = stitch(mapShape, *piece);

			// copy for expected piece pos
			/*
			std::unique_ptr<Shape> expectedPiecePos;
			expectedPiecePos = std::make_unique<Shape>(piece);
			expectedPiecePos.get()->posY = 15;
			wholeMapShape = stitch(mapShape, *expectedPiecePos);
			*/
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
			//TODO  we need check for rotation to prevent invalid moves
			piece->rotate();
			wholeMapShape = (stitch(mapShape, *piece));
		}

		if (Input::getKeyPressed(Input::Key::SPACE)) {
			
			move(*piece, 0, 1);
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
	mutex.lock();
	Shape stitchedShape = shape1; // Start with the base map
	for (int y = 0; y < piece.height; y++) {
		for (int x = 0; x < piece.width; x++) {
			if (piece.shape[x + y * piece.width] != '*') {
				stitchedShape.shape[x+piece.posX + (y + piece.posY) * width] = piece.shape[x + y * piece.width];
			}
		}
	}
	mutex.unlock();
	return stitchedShape;
}
