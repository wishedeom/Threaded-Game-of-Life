#pragma once

#include <string>

#include <GL/glew.h>

class Shader
{
public:
	const GLuint id;

	Shader(const std::string& vs_path, const std::string& fs_path);
	
	void use();
};
