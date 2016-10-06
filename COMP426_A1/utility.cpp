#include "stdafx.h"
#include "utility.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"

glm::vec4 rand_colour()
{
	const auto r = []() -> float { return static_cast<float>(std::rand()) / RAND_MAX; };
	return { r(), r(), r(), 1.0f };
}

std::string read_file(const std::string& path)
{
	std::ifstream file;
	std::stringstream stream;

	file.exceptions(std::ifstream::badbit);
	try
	{
		file.open(path);
		stream << file.rdbuf();
		file.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR: File " << path << " read failure." << std::endl;
	}

	return stream.str();
}