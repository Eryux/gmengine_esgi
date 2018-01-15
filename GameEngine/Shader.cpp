#include "Shader.h"

#include "GL\glew.h"

#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Engine;

bool checkShader(GLuint shader)
{
	int didCompile = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &didCompile);
	if (!didCompile)
	{
		char logBuffer[4096];
		int len = 0;
		glGetShaderInfoLog(shader, 4096, &len, logBuffer);

#ifdef _WIN32
		OutputDebugStringA(logBuffer);
#endif

		std::cout << logBuffer << std::endl;
	}
	return didCompile;
}

void Shader::DestroyProgram(unsigned int i)
{
	shader_t shader = m_shaders[i];

	glDetachShader(shader.program, shader.vertex_shader);
	glDetachShader(shader.program, shader.fragment_shader);
	glDeleteProgram(shader.program);

	glDeleteShader(shader.vertex_shader);
	glDeleteShader(shader.fragment_shader);
}

void Shader::Free() {
	for (unsigned int i = 0; i < m_shaders.size(); i++) {
		DestroyProgram(i);
	}
}

bool checkProgram(GLuint program)
{
	int didLink = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &didLink);
	if (!didLink)
	{
		char logBuffer[4096];
		int len = 0;
		glGetProgramInfoLog(program, 4096, &len, logBuffer);

#ifdef _WIN32
		OutputDebugStringA(logBuffer);
#endif

		std::cout << logBuffer << std::endl;
	}
	return didLink;
}

uint32_t Shader::CreateProgram(uint32_t vertex_shader, uint32_t fragment_shader)
{
	uint32_t programObject = glCreateProgram();

	glAttachShader(programObject, vertex_shader);
	glAttachShader(programObject, fragment_shader);
	glLinkProgram(programObject);
	checkProgram(programObject);

	return programObject;
}

uint32_t Shader::CompileShader(uint32_t type, const char* code)
{
	uint32_t shaderObject = glCreateShader(type);
	if (shaderObject != 0) {
		glShaderSource(shaderObject, 1, &code, nullptr);
		glCompileShader(shaderObject);
	}

	return shaderObject;
}

int Shader::LoadShader(const char * vertex_shader_file, const char * frament_shader_file) 
{
	shader_t shader;

	shader.vertex_shader = LoadVertexShader(vertex_shader_file);
	shader.fragment_shader = LoadFragmentShader(frament_shader_file);

	if (shader.vertex_shader < 1) {
		std::cerr << "Unable to load vertex shader : " << vertex_shader_file << std::endl;
		return -1;
	}
	if (shader.fragment_shader < 1) {
		std::cerr << "Unable to load fragment shader : " << frament_shader_file << std::endl;
		return -1;
	}

	shader.program = CreateProgram(shader.vertex_shader, shader.fragment_shader);
	m_shaders.push_back(shader);
	return m_shaders.size() - 1;
}

uint32_t Shader::LoadVertexShader(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);

	auto len = ftell(file);
	rewind(file);

	auto buffer = new char[len + 1];
	fread(buffer, len, 1, file);

	buffer[len] = '\0'; // = 0
	uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, buffer);
	delete[] buffer;

	checkShader(vertexShader);
	return vertexShader;
}

uint32_t Shader::LoadFragmentShader(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);

	auto len = ftell(file);
	rewind(file);

	auto buffer = new char[len + 1];
	fread(buffer, len, 1, file);

	buffer[len] = '\0';
	uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, buffer);
	delete[] buffer;

	checkShader(fragmentShader);
	return fragmentShader;
}