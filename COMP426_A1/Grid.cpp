#include "stdafx.h"
#include "Grid.h"

#include <algorithm>

#include "glm/glm.hpp"

Grid::Grid(int aWidth, int aHeight)
	: width(aWidth)
	, height(aHeight)
	, area(aWidth * aHeight)
	, _squares(area)
{}

GridSquare & Grid::grid_square(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		throw std::out_of_range("Trying to access a square outside the grid.");

	return _squares[x + y * width];
}

bool& Grid::operator()(int x, const int y)
{
	return grid_square(x, y).is_alive;
}

unsigned int Grid::live_neighbours(int x, int y)
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

void Grid::update_is_alive(int x, int y)
{
	auto& square = grid_square(x, y);
	square.is_alive = square.will_be_alive;
}

void Grid::update_will_be_alive(int x, int y)
{
	auto& square = grid_square(x, y);
	const auto count = live_neighbours(x, y);

	square.will_be_alive
		= (square.is_alive && (count == 2 || count == 3))
		|| (!square.is_alive && count == 2);
}

void Grid::update_is_alive_columns(int first, int last)
{
	for (int x = first; x <= last; x++)
	{
		for (int y = 0; y < height; y++)
		{
			update_is_alive(x, y);
		}
	}
}

void Grid::update_will_be_alive_columns(int first, int last)
{
	for (int x = first; x <= last; x++)
	{
		for (int y = 0; y < height; y++)
		{
			update_will_be_alive(x, y);
		}
	}
}

std::vector<glm::vec3> grid_vertices(int width, int height, float border_thickness, glm::vec4 colour)
{
	const auto max_dim = std::max(width, height);
	const auto min_dim = std::min(width, height);

	float lwidth;
	float lheight;

	if (width >= height)
	{
		lwidth = 2.0f * (1.0f - border_thickness);
		lheight = lwidth * height / width;
	}

	return std::vector<glm::vec3>();
}
