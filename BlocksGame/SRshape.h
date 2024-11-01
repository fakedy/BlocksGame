#pragma once
#include "Shape.h"


class SRshape : public Shape {
public:

	SRshape() {

		width = 2;
		height = 3;
		posX = 4;
		posY = 0;

		shape = {
		'*','5',
		'5','5',
		'5','*'
		};
	}

};