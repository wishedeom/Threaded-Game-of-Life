#pragma once

#include <string>

#include <GL/glew.h>

class Shader
{
public:
	Shader(const std::string& vs_path, const std::string& fs_path);
	const GLuint id;
};
