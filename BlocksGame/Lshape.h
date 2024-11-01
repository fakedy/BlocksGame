#pragma once
#include "Shape.h"


class Lshape : public Shape {
public:
	Lshape() {

		width = 2;
		height = 3;
		posX = 4;
		posY = 0;

		shape = {
		'2','*',
		'2','*',
		'2','2'
		};
	}

};