#include "stdafx.h"

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include "utility.h"

GLuint create_shader(const std::string& path, const GLenum shader_type);
GLuint create_shader_program(const std::string& vs_path, const std::string& fs_path);

Shader::Shader(const std::string& vs_path, const std::string& fs_path)
	: id(create_shader_program(vs_path, fs_path))
{}

void Shader::use()
{
	glUseProgram(id);
}

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
