#include "Shape.h"

void Shape::transpose()
{


	std::vector<char> transposed(width * height);
    // swap rows with columns in matrix
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            transposed[y + x * height] = shape[x + y * width];
        }
    }

    // swapping height and width
	int tempWidth = width;
	width = height; 
	height = tempWidth;
    shape = std::move(transposed); // replace original shape with transposed one
	// do something
}

void Shape::reverse() {

    // in place modification to reverse the vector
    for (size_t x = 0; x < width/2; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            std::swap(shape[x + y * width], shape[(width - 1 - x) + y * width]);
        }
    }
}

void Shape::rotate() {

    // a clockwise rotation by transposing then reversing
    transpose();
    reverse();
}

