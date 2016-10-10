#include "stdafx.h"
#include "Grid.h"

#include <algorithm>
#include <cstdlib>
#include <thread>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Shader.h"

// Degree of multiprogramming
const size_t num_threads = 10;


// Constructs a grid and prepares all drawing buffers
Grid::Grid(const int width, const int height, glm::vec4 colour)
	: width(width)
	, height(height)
	, area(width * height)
	, colour(colour)
	, vertices(compute_vertices())
	, _squares(area)
{
	glGenVertexArrays(1, &_vao_id);
	glGenBuffers(1, &_vbo_id);
	glGenBuffers(1, &_ebo_id);

	glBindVertexArray(_vao_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	update_ebo();
}


// Cleans up buffers
Grid::~Grid()
{
	glDeleteVertexArrays(1, &_vao_id);
	glDeleteBuffers(1, &_vbo_id);
	glDeleteBuffers(1, &_ebo_id);
}


// Gets a grid square from coordinates
GridSquare& Grid::grid_square(int x, int y)
{
	return _squares[x + y * width];
}


// Gets a grid square from coordinates
GridSquare& Grid::grid_square(int2 coords)
{
	return grid_square(coords.x, coords.y);
}


// Gets grid coordinates from a vector index
int2 Grid::grid_coords(int idx) const
{
	return { idx % width, idx / width };
}


// Returns the number of live neighbours of a grid cell
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


// Returns the number of live neighbours of a grid cell
unsigned int Grid::live_neighbours(int idx)
{
	const auto coords = grid_coords(idx);
	return live_neighbours(coords.x, coords.y);
}


// Computes grid vertices from width and height
std::vector<glm::vec3> Grid::compute_vertices()
{
	float x0;
	float y0;
	float grid_width;

	if (width >= height)
	{
		grid_width = 2.0f;
		x0 = -1.0f;
		y0 = grid_width * height / width / 2;
	}
	else
	{
		grid_width = 2.0f * width / height;
		x0 = -grid_width / 2;
		y0 = 1.0f;
	}

	_side_length = grid_width / width;

	std::vector<glm::vec3> vertices;
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			vertices.push_back(glm::vec3(x0 + x * _side_length, y0 - y * _side_length, 0.0f));
		}
	}

	return vertices;
}


// Computes drawing indices
std::vector<int> Grid::compute_indices()
{
	std::vector<int> indices;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (grid_square(x, y).is_alive)
			{
				const auto iw = width + 1;		// Number of indices per row
				
				const auto ul = x + iw * y;		// Upper left
				const auto ur = ul + 1;			// Upper right
				const auto ll = ul + iw;		// Lower left
				const auto lr = ll + 1;			// Lower right
				
				indices.push_back(ul);
				indices.push_back(ur);
				indices.push_back(ll);
				indices.push_back(lr);
				indices.push_back(ll);
				indices.push_back(ur);
			}
		}
	}

	return indices;
}


// Caches drawing indices
void Grid::update_indices()
{
	_indices = compute_indices();
}


// Sets each cell alive or dead with a 0.5 probability
void Grid::populate_random()
{
	for (auto& square : _squares)
	{
		square.is_alive = std::rand() % 2 == 0;
	}
}


// Sets cells alive in a disk with a specified centre and radius
void Grid::populate_disk(float radius, float h, float k)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			const auto dx = x - h;
			const auto dy = y - k;
			if (dx * dx + dy * dy <= radius * radius)
			{
				grid_square(x, y).is_alive = true;
			}
		}
	}
}


// Updates the index buffer, for use after the indices have been changed
void Grid::update_ebo()
{
	glBindVertexArray(_vao_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


// Draw the grid with the specified shader
void Grid::draw(const Shader& shader) const
{
	shader.use(colour);
	glBindVertexArray(_vao_id);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


// Update the grid according to the rules
void Grid::update()
{
	update_will_be_alive();
	update_is_alive();
	update_indices();
	update_ebo();
}


// Get world coordinates if a square from a vector index
glm::vec2 Grid::square_coords(int idx) const
{
	const auto coords = grid_coords(idx);
	return{ (coords.x + 0.5f) * _side_length, (coords.y + 0.5f) * _side_length };
}


// Update each cell's status for the current turn
void Grid::update_is_alive()
{
	_alive_squares.clear();
	split_on_threads(&Grid::update_is_alive_indices);
}


// Update cells within the specified indices
void Grid::update_is_alive_indices(size_t begin, size_t end)
{
	for (size_t i = begin; i < end; i++)
	{
		auto& square = _squares[i];
		if (square.is_alive = square.will_be_alive)	// Intentional assignment
		{
			_alive_squares.emplace_back(square_coords(i));
		}
	}
}


// Update each cell's status for the next turn based on the number of live neighbours
void Grid::update_will_be_alive()
{
	split_on_threads(&Grid::update_will_be_alive_indices);
}


// Update cells within the specified indices
void Grid::update_will_be_alive_indices(size_t begin, size_t end)
{
	for (size_t i = begin; i < end; i++)
	{
		auto& square = _squares[i];
		const auto count = live_neighbours(i);
		square.will_be_alive
			= (square.is_alive && (count == 2 || count == 3))
			|| (!square.is_alive && count == 2);
	}
}


// Execute one of the two update functions by splitting all vector indices among some number of threadss
void Grid::split_on_threads(void(Grid::*f)(size_t, size_t))
{
	std::vector<std::thread> threads;
	auto prev_begin = 0;
	for (size_t i = 0; i < num_threads; i++)
	{
		const auto size = _squares.size();
		const auto n = size / num_threads + (i == 0 ? size % num_threads : 0);
		const auto end = prev_begin + n;
		threads.push_back(std::thread(f, this, prev_begin, end));
		prev_begin = end;
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}