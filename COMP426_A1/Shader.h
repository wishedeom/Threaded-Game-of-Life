#pragma once

#include <string>

#include <GL/glew.h>
#include "glm/glm.hpp"

class Shader
{
public:
	const GLuint id;
	const GLuint colour_loc;

	Shader(const std::string& vs_path, const std::string& fs_path);
	
	void use(const glm::vec4 colour) const;
};
