#pragma once
#include "Shape.h"


class LRshape : public Shape {
public:
	LRshape() {

		width = 2;
		height = 3;
		posX = 4;
		posY = 0;

		shape = {
		'*','3',
		'*','3',
		'3','3'
		};
	}

};