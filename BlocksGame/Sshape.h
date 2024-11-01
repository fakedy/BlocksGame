#pragma once
#include "Shape.h"


class Sshape : public Shape {
public:
	Sshape() {

		width = 2;
		height = 3;
		posX = 4;
		posY = 0;

		shape = {
		'4','*',
		'4','4',
		'*','4'
		};
	}

};