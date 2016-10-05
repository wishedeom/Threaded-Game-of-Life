#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "GridSquare.h"

class Grid
{
public:
	const int width;
	const int height;
	const int area;
	const std::vector<glm::vec3> vertices;

	Grid(int width, int height);
	GridSquare& grid_square(int x, int y);
	bool& operator()(int x, int y);
	unsigned int live_neighbours(int x, int y);
	void update_will_be_alive_columns(int first, int last);
	void update_is_alive_columns(int first, int last);
	std::vector<int> indices();
	void populate_random();
	void populate_disk(float radius);
	void bind_vao();

private:
	std::vector<GridSquare> _squares;
	std::vector<int> _indices;
	bool _up_to_date = false;
	GLuint _vao_id;
	GLuint _vbo_id;
	GLuint _ebo_id;

	void update_will_be_alive(int x, int y);
	void update_is_alive(int x, int y);
	std::vector<glm::vec3> compute_vertices();
	std::vector<int> compute_indices();
	void update_ebo();
};
