#pragma once

#include <vector>

#include "GridSquare.h"

class Grid
{
public:
	const int width;
	const int height;
	const int area;

	Grid(const int& aWidth, const int& aHeight);
	GridSquare& grid_square(const int& x, const int& y);
	bool& operator()(const int& x, const int& y);
	unsigned int live_neighbours(const int& x, const int& y);
	void update_will_be_alive_columns(const int& first, const int& last);
	void update_is_alive_columns(const int& first, const int& last);

private:
	std::vector<GridSquare> _squares;

	void update_will_be_alive(const int& x, const int& y);
	void update_is_alive(const int& x, const int& y);
};
