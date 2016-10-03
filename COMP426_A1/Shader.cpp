#include "stdafx.h"

#include "Shader.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include "utility.h"

GLuint create_shader(const std::string& path, const GLenum shader_type);
GLuint create_shader_program(const std::string& vs_path, const std::string& fs_path);

Shader::Shader(const std::string& vs_path, const std::string& fs_path)
	: id(create_shader_program(vs_path, fs_path))
{}

GLuint create_shader(const std::string& path, const GLenum shader_type)
{
	const GLuint id = glCreateShader(shader_type);
	const auto source = read_file(path).c_str();
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);
	return id;
}

GLuint create_shader_program(const std::string& vs_path, const std::string& fs_path)
{
	const GLuint vertex_shader_id = create_shader(vs_path, GL_VERTEX_SHADER);
	const GLuint fragment_shader_id = create_shader(fs_path, GL_FRAGMENT_SHADER);
	
	const GLuint shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);
	glLinkProgram(shader_program_id);
	
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return shader_program_id;
}