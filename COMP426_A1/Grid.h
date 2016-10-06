#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "GridSquare.h"
#include "Shader.h"

struct int2
{
	int x;
	int y;
};

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
	bool& operator()(int x, int y);
	unsigned int live_neighbours(int x, int y);
	unsigned int live_neighbours(int idx);
	std::vector<int> indices();
	void populate_random();
	void populate_disk(float radius, float x, float y);
	void update_ebo();
	void draw(const Shader& shader) const;
	void update();

private:
	std::vector<GridSquare> _squares;
	std::vector<int> _indices;
	bool _up_to_date = false;
	GLuint _vao_id;
	GLuint _vbo_id;
	GLuint _ebo_id;

	void update_will_be_alive();
	void update_is_alive();
	std::vector<glm::vec3> compute_vertices();
	std::vector<int> compute_indices();
	void update_indices();
};
