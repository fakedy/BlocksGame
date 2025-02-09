#pragma once
#include "Shape.h"


class Ishape : public Shape {
public:
	Ishape() {

		width = 4;
		height = 1;
		posX = 4;
		posY = 0;

		shape = {
		'0','0','0','0'
		};
	}

};