#pragma once
#include "Shape.h"


class Tshape : public Shape {
public:
	Tshape() {

		width = 3;
		height = 2;
		posX = 4;
		posY = 0;

		shape = {
			'*','x','*',
			'x','x','x',
		};
	}

};