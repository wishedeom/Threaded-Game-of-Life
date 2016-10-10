#include "stdafx.h"

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utility.h"


// Function prototypes
GLuint create_shader(const std::string& path, const GLenum shader_type);
GLuint create_shader_program(const std::string& vs_path, const std::string& fs_path);


// Creates a shader
Shader::Shader(const std::string& vs_path, const std::string& fs_path)
	: id(create_shader_program(vs_path, fs_path))
	, colour_loc(glGetUniformLocation(id, "u_colour"))
{}


// Binds the shader for use
void Shader::use(const glm::vec4 colour) const
{
	glUseProgram(id);
	if (colour_loc != -1)
	{
		glUniform4fv(colour_loc, 1, glm::value_ptr(colour));
	}
}


// Ceates a shader and returns its id
GLuint create_shader_program(const std::string& vs_path, const std::string& fs_path)
{
	const auto shader_program_id = glCreateProgram();

	const auto vs_source = read_file(vs_path);
	const auto vertex_shader_id = create_shader(vs_source, GL_VERTEX_SHADER);
	glAttachShader(shader_program_id, vertex_shader_id);

	const auto fs_source = read_file(fs_path);
	const auto fragment_shader_id = create_shader(fs_source, GL_FRAGMENT_SHADER);
	glAttachShader(shader_program_id, fragment_shader_id);

	glLinkProgram(shader_program_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return shader_program_id;
}


// Creates a vertex or fragment shader and returns its id
GLuint create_shader(const std::string& source_string, const GLenum shader_type)
{
	const auto source = source_string.c_str();
	
	const auto id = glCreateShader(shader_type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		const auto length = 512;
		GLchar info_log[length];
		glGetShaderInfoLog(id, length, NULL, info_log);
		const auto shader_type_name = shader_type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
		std::cout << "Error: " << shader_type_name << " shader compilation error.\n" << info_log << std::endl;
	}

	return id;
}
