#include "stdafx.h"

#include "utility.h"

#include <fstream>
#include <string>
#include <sstream>

std::string read_file(const std::string& path)
{
	std::ifstream in(path);
	std::string line;
	std::stringstream ss;
	while (std::getline(in, line))
	{
		ss << line << "\n";
	}
	return ss.str();
}