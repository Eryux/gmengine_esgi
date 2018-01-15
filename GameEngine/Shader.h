#pragma once

#ifndef UTILS_SHADER
#define UTILS_SHADER

#include <vector>
#include <cstdint>

namespace Engine {

	typedef struct {
		uint32_t program;
		uint32_t vertex_shader;
		uint32_t fragment_shader;
	} shader_t;

	class Shader
	{

	public:
		Shader() {}
		~Shader() {}

		int LoadShader(const char * vertex_shader_file, const char * frament_shader_file);

		inline uint32_t GetProgram(int i) { return m_shaders[i].program; }
		inline shader_t * GetShader(int i) { return &m_shaders[i]; }

		void DestroyProgram(unsigned int i);
		void Free();

	private:
		std::vector<shader_t> m_shaders;

		uint32_t LoadVertexShader(const char * filename);
		uint32_t LoadFragmentShader(const char* filename);

		uint32_t CompileShader(uint32_t type, const char* code);
		uint32_t CreateProgram(uint32_t vertex_shader, uint32_t fragment_shader);

	};

}

#endif