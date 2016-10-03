#include "stdafx.h"

#include "Grid.h"

Grid::Grid(const int& aWidth, const int& aHeight)
	: width(aWidth), height(aHeight), area(aWidth * aHeight)
{
	_squares = std::vector<GridSquare>(area);
}

GridSquare & Grid::grid_square(const int& x, const int& y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		throw std::out_of_range("Trying to access a square outside the grid.");

	return _squares[x + y * width];
}

bool& Grid::operator()(const int& x, const int& y)
{
	return grid_square(x, y).is_alive;
}

unsigned int Grid::live_neighbours(const int& x, const int& y)
{
	unsigned int count = 0u;

	if (x > 0)
	{
		if (y > 0)
			count += grid_square(x - 1, y - 1).is_alive;

		count += grid_square(x - 1, y).is_alive;

		if (y < height - 1)
			count += grid_square(x - 1, y + 1).is_alive;
	}

	if (y > 0)
		count += grid_square(x, y - 1).is_alive;

	count += grid_square(x, y).is_alive;

	if (y < height - 1)
		count += grid_square(x, y + 1).is_alive;

	if (x < width - 1)
	{
		if (y > 0)
			count += grid_square(x + 1, y - 1).is_alive;

		count += grid_square(x + 1, y).is_alive;

		if (y < height - 1)
			count += grid_square(x + 1, y + 1).is_alive;
	}

	return count;
}

void Grid::update_is_alive(const int& x, const int& y)
{
	auto& square = grid_square(x, y);
	square.is_alive = square.will_be_alive;
}

void Grid::update_will_be_alive(const int& x, const int& y)
{
	auto& square = grid_square(x, y);
	const auto count = live_neighbours(x, y);

	square.will_be_alive
		= (square.is_alive && (count == 2 || count == 3))
		|| (!square.is_alive && count == 2);
}

void Grid::update_is_alive_columns(const int& first, const int& last)
{
	for (int x = first; x <= last; x++)
	{
		for (int y = 0; y < height; y++)
		{
			update_is_alive(x, y);
		}
	}
}

void Grid::update_will_be_alive_columns(const int& first, const int& last)
{
	for (int x = first; x <= last; x++)
	{
		for (int y = 0; y < height; y++)
		{
			update_will_be_alive(x, y);
		}
	}
}
