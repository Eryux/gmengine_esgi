#pragma once

#ifndef ENGINE_SKYBOX_H
#define ENGINE_SKYBOX_H


#include "Engine.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Image.hpp>


namespace Engine {

	class Skybox {

	private:
		GLuint m_vbo;

		GLuint m_vao;

		GLuint m_tex;

		uint32_t m_render_program;
		
		Core * m_core;

		bool loadTextureSide(GLenum side, std::string tex_file);

	public:
		Skybox();

		bool loadTexture(std::string tex_path);

		void setShader(uint32_t program);

		void draw();

	};

}


#endif // !ENGINE_SKYBOX_H
