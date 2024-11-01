#pragma once
#include "Shape.h"


class Oshape : public Shape {
public:
	Oshape() {

		width = 2;
		height = 2;
		posX = 4;
		posY = 0;

		shape = {
		'6','6',
		'6','6',
		};
	}

};