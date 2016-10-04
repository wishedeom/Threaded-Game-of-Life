#pragma once

#include <vector>

#include "GridSquare.h"

class Grid
{
public:
	const int width;
	const int height;
	const int area;

	Grid(int aWidth, int aHeight);
	GridSquare& grid_square(int x, int y);
	bool& operator()(int x, int y);
	unsigned int live_neighbours(int x, int y);
	void update_will_be_alive_columns(int first, int last);
	void update_is_alive_columns(int first, int last);

private:
	std::vector<GridSquare> _squares;

	void update_will_be_alive(int x, int y);
	void update_is_alive(int x, int y);
};
