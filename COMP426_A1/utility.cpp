#include "stdafx.h"

#include "utility.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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