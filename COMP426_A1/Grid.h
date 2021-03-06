#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "GridSquare.h"
#include "Shader.h"

// Integer vector
struct int2
{
	int x;
	int y;
};

// Represents a single species on the grid of Game of Life
class Grid
{
public:
	const int width;
	const int height;
	const int area;
	const glm::vec4 colour;
	const std::vector<glm::vec3> vertices;

	Grid(int width, int height, glm::vec4 colour);
	~Grid();

	GridSquare& grid_square(int x, int y);
	GridSquare& grid_square(int2 coords);
	int2 grid_coords(int idx) const;
	glm::vec2 square_coords(int idx) const;
	unsigned int live_neighbours(int x, int y);
	unsigned int live_neighbours(int idx);
	void populate_random();
	void populate_disk(float radius, float x, float y);
	void update_ebo();
	void draw(const Shader& shader) const;
	void update();

private:
	std::vector<GridSquare> _squares;
	std::vector<int> _indices;
	std::vector<glm::vec2> _alive_squares;
	GLuint _vao_id;
	GLuint _vbo_id;
	GLuint _ebo_id;
	float _side_length;

	void update_will_be_alive();
	void update_will_be_alive_indices(size_t begin, size_t end);
	void update_is_alive();
	void update_is_alive_indices(size_t begin, size_t end);
	std::vector<glm::vec3> compute_vertices();
	std::vector<int> compute_indices();
	void update_indices();
	void split_on_threads(void (Grid::*f)(size_t, size_t));
};
