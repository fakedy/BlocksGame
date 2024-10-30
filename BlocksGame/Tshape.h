#pragma once
#include "Shape.h"


class Tshape : public Shape {
public:
	Tshape() {

		width = 3;
		height = 2;
		posX = 4;
		posY = 0;



		// changed with to 10 to test something with stitching
		/*
		shape = {
			'*','*','*','*','x','*','*','*','*', '*',
			'*','*','*','x','x','x','*','*','*', '*'
		};
		*/

		// this is what cause crash or issues

		shape = {
			'*','x','*',
			'x','x','x',
		};
	}

};