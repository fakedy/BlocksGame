#include "Shape.h"

void Shape::transpose()
{
	std::vector<char> temp(width * height);
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            temp[y + x * height] = shape[x + y * width];
        }
    }

	int tempWidth = width;
	width = height;
	height = tempWidth;
    shape = std::move(temp);
	// do something
}
