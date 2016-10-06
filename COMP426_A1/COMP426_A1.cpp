#include "stdafx.h"

#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Grid.h"
#include "GridSquare.h"
#include "Shader.h"
#include "utility.h"

#define GLEW_STATIC

// Function prototypes
void initialize_openGL();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint window_width = 1024;
const GLuint window_height = 768;

// Window
GLFWwindow* window;

// Grid dimensions
const int grid_width = 100;	// 1024
const int grid_height = 100;	// 768

// Limits on number of species
const size_t min_species = 5;
const size_t max_species = 10;

// Screen refresh rate in s^-1
const double update_rate = 30.0;
const std::chrono::milliseconds update_time(static_cast<int>(1000 / update_rate));

int main()
{
	std::cout << "How many species? (" << min_species << " - " << max_species << ")" << std::endl;
	size_t num_species;
	std::cin >> num_species;
	if (!(min_species <= num_species && num_species <= max_species))
	{
		std::cout << "Can you read?" << std::endl;
		exit(1);
	}

	std::srand(static_cast<unsigned int>(std::time(0)));

	initialize_openGL();

	Shader shader("vertex_shader.vs", "fragment_shader.fs");

	std::vector<Grid> grids;
	for (size_t i = 0; i < num_species; i++)
	{
		grids.emplace_back(grid_width, grid_height, rand_colour());
	}

	for (auto& grid : grids)
	{
		grid.populate_disk(10, std::rand() % grid_width, std::rand() % grid_height);
	}

	while (!glfwWindowShouldClose(window))
	{
		const auto start = std::chrono::steady_clock::now();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (auto& grid : grids)
		{
			grid.draw(shader);
		}
		
		glfwSwapBuffers(window);
		
		for (auto& grid : grids)
		{
			grid.update();
		}
		while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start) < update_time) {}
	}
	glfwTerminate();
	return 0;
}


void initialize_openGL()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
