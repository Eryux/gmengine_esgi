#pragma once

#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "Component.h"
#include "Transform.h"
#include "Shader.h"

#include "GL/glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif

#include <SFML/OpenGL.hpp>

#include "tiny_obj_loader.h"

namespace Engine {

	class Core;

	typedef struct {
		GLfloat * vertices = nullptr;
		GLuint * indexes = nullptr;

		unsigned int vertices_size = 0;
		unsigned int indexes_size = 0;

		int vbo_ibo_index = -1;

		std::string name;

		tinyobj::attrib_t * data = nullptr;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
	} model_t;

	class Renderer : public Component {

	private:
		static std::vector<model_t*> s_models;

		Transform * m_transform;

		shader_t * m_shader;

		int m_material_id = -1;

		int m_texture_id = -1;

		int m_shader_id = -1;

		std::string m_model_name;

		std::string m_material_path;

		std::string m_texture_name = "none";

		Core * m_core = nullptr;

	public:
		static int s_LAMBERT_SHADER;

		model_t * m_model;

		Renderer();
		Renderer(std::string model_path);
		Renderer(std::string model_path, std::string material_path);

		void SetShader(shader_t * s);
		shader_t * GetShader();

		static void CompileForOpenGL(std::string model_path);
		static bool IsModelLoaded(std::string model_path);
		static bool LoadModel(std::string model_path, std::string material_path);
		static void FreeModel(std::string model_path);
		static void FreeModelAll();

		void SetModel(std::string model_path);

		void SetMaterial(int i);
		tinyobj::material_t * GetMaterial(int i);

		void SetTexture(int i);

		model_t * GetModel();

		void draw();
		void start();
		void remove();

	};

};

#endif