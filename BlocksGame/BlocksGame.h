#pragma once
#include <array>
#include <span>
#include <vector>
#include "Tshape.h"
#include "Ishape.h"
#include <mutex>
#include <memory>
#include <functional>
#include "Tshape.h"
#include "Ishape.h"
#include "Lshape.h"
#include "LRshape.h"
#include "Sshape.h"
#include "SRshape.h"
#include "Oshape.h"

static std::mutex mutex;

class BlocksGame
{

public:

	static const int width{ 10 };
	static const int height{ 20 };
	int score{ 0 };
	static Shape wholeMapShape; // static :8
	BlocksGame();

	void tick();
	void update();
	void stop();

private:
	bool usedHeld{false};
	bool gameShouldStop{false};
	bool hasmoved{ false };
	Shape mapShape{};
	std::unique_ptr<Shape> piece;
	std::unique_ptr<Shape> heldPiece;
	bool m_paused{false};
	
	constexpr std::vector<char> fillMap();
	int rowClear(Shape& map);
	void blockHitBottom();
	bool canMove(const Shape& map, const Shape piece);
	bool move(Shape& shape, int x, int y);
	Shape stitch(const Shape& shape1, const Shape& shape2);

	std::vector<std::function<std::unique_ptr<Shape>()>> shapeCreators = {
	[]() { return std::make_unique<Tshape>(); },
	[]() { return std::make_unique<Ishape>(); },
	[]() { return std::make_unique<Lshape>(); },
	[]() { return std::make_unique<LRshape>(); },
	[]() { return std::make_unique<SRshape>(); },
	[]() { return std::make_unique<Sshape>(); },
	[]() { return std::make_unique<Oshape>(); },

	};



};

