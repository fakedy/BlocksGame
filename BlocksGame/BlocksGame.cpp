#include "BlocksGame.h"
#include <iostream>
#include <thread>
#include <vector>
#include "Input.h"
#include <random>


Shape BlocksGame::wholeMapShape;
std::random_device rd; // random seed
std::mt19937 gen(rd()); // init generator with random seed
std::uniform_int_distribution<int> dist(1, 100); // defines our distribution

constexpr std::vector<char> BlocksGame::fillMap() {
	
	std::vector<char> array;
	array.reserve(height * width);
	array.resize(height * width);

	for (int y = 0; y < height; y++) {

		for (int x = 0; x < width; x++)
		{
			int index = x + y * width;
			array[index] = '*';
		}

	}

	return array;
}

BlocksGame::BlocksGame(ma_engine &soundEngine) : soundEngine(soundEngine){
	// create the base shape 
	mapShape.shape = fillMap();
	mapShape.width = width;
	mapShape.height = height;


	wholeMapShape.shape = fillMap();
	wholeMapShape.width = width;
	wholeMapShape.height = height;

	// randomly select first piece
	piece = shapeCreators[dist(gen) % shapeCreators.size()]();
	wholeMapShape = stitch(mapShape, *piece);
	ma_engine_play_sound(&soundEngine, "Resources/Sounds/outlands.wav", 0);
}

void BlocksGame::tick() {



	static auto lastUpdate = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	auto tickElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();

	if (tickElapsed > 1000) {
		lastUpdate = now;
		if (!m_paused) {
			// if manage to move down
			if (!move(*piece, 0, 1) && piece != nullptr) {
				blockHitBottom();
			}
		}
	}

}

void BlocksGame::blockHitBottom() {
	mapShape = stitch(mapShape, *piece);
	wholeMapShape = stitch(mapShape, *piece);
	int score = rowClear(wholeMapShape); // check if we have a full row. gives an int as return (score)
	if (score == 0) {
		// play sound
		ma_engine_play_sound(&soundEngine, "Resources/Sounds/click.wav", 0);
	}
	else {
		ma_engine_play_sound(&soundEngine, "Resources/Sounds/490611.wav", 0);
	}
	mapShape = wholeMapShape; // after clearing full rows transfer state to mapShape
	piece = nullptr;
	usedHeld = false;
	tickElapsed = std::chrono::milliseconds(0);
}

int BlocksGame::rowClear(Shape& map) {
	int counter = 0;
	std::vector<int> locations;
	char prev = '*';

	for (int y = 0; y < map.height; y++) {
		for (int x = 0; x < map.width; x++) {
			
			int index = x + y * map.width;
			if (map.shape[index] != '*' && map.shape[index] != 'x') {
				// if we are at start of row or previous one was a block
				if (index % map.width == 0 || (prev != '*' && prev != 'x')) {
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
	
	for (int i = 0; i < locations.size(); i++)
	{
		map.shape.insert(map.shape.begin(), map.width ,'*');
	}

	score += locations.size();
	

	return counter;

}

void BlocksGame::update() {

	// main thread
	if (Input::getKeyPressed(Input::Key::ESCAPE)) {
		m_paused = !m_paused;
	}
	
	if (m_paused) {
		return;
	}

	// if we have no piece, create it.
	if (piece == nullptr) {
		piece = shapeCreators[dist(gen) % shapeCreators.size()]();
		wholeMapShape = stitch(mapShape, *piece);
	}


	// detect input and move piece
	if (hasmoved) {



		// stitch piece onto the map
		wholeMapShape = stitch(mapShape, *piece);

			
		// copy for expected piece pos
		std::unique_ptr<Shape> expectedPiecePos;
		expectedPiecePos = std::make_unique<Shape>(*piece);
		// this is to replace this expectedPiecePos to be x where x is rendered as marker for piece drop
		for (int x = 0; x < expectedPiecePos.get()->width; x++)
		{
			for (int y = 0; y < expectedPiecePos.get()->height; y++)
			{
				int index = x + y * expectedPiecePos.get()->width;
				if (expectedPiecePos.get()->shape[index] != '*') {
					expectedPiecePos.get()->shape[index] = 'x';
				}
			}
		}

		while (move(*expectedPiecePos, 0, 1));
		// stitch this piece for display
		wholeMapShape = stitch(wholeMapShape, *expectedPiecePos);
			

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

		// low effort fix
		piece->rotate();
		if (!canMove(mapShape, *piece)) {
			piece->rotate();
		}
		hasmoved = true;
	}

	if (Input::getKeyPressed(Input::Key::SPACE)) {
			
		// while move returns true keep going down
		while (move(*piece, 0, 1));
		blockHitBottom();
	}

	if (Input::getKeyPressed(Input::Key::C)) {
		
		if (!usedHeld) {

			if (heldPiece == nullptr) {
				heldPiece = std::move(piece);
			}
			else {
				std::unique_ptr<Shape> temp = std::move(piece);
				piece = std::move(heldPiece);
				heldPiece = std::move(temp);

				piece.get()->posY = 0;
				piece.get()->posX = 4;
			}
			usedHeld = true;
			tickElapsed = std::chrono::milliseconds(0);
		}

		hasmoved = true;
		
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
			int index = x + y * piece.width;
			int mapIndex = x + piece.posX + (y + piece.posY) * width;
			if ((piece.shape[index] != '*' && (map.shape[mapIndex] != '*' &&  map.shape[mapIndex] != 'x'))) {
				return false;
			}

		}
	}

	return true;
}

bool BlocksGame::move(Shape& shape, int x, int y) {
	// Polymorphic Copying
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
			int index = x + y * piece.width;
			int stitchedIndex = x + piece.posX + (y + piece.posY) * width;
			if (piece.shape[index] != '*') {
				// do not overwrite something that int a * with an x
				if (piece.shape[index] != 'x' || (piece.shape[index] == 'x' && stitchedShape.shape[stitchedIndex] == '*')) {
					stitchedShape.shape[stitchedIndex] = piece.shape[index];
				}
			}
		}
	}
	return stitchedShape;
}

void BlocksGame::stop() {
	gameShouldStop = true;
}
