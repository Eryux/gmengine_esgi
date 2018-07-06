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
#include <fbxsdk.h>

#include "tiny_obj_loader.h"

namespace Engine {

	class Core;

	typedef struct {
		std::string name;
		FbxNode * node;
	} bone_t;

	typedef struct {
		std::vector<bone_t> bones;
	} skeleton_t;

	typedef struct {
		std::vector<glm::mat4> joints;
	} frame_t;

	typedef struct {
		std::vector<frame_t> keyframes;
	} anim_t;

	typedef struct {
		std::vector<int> jointIndex;
		std::vector<float> weights;
	} deformer_t;

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

		skeleton_t * skeleton;
		std::vector<glm::mat4> bindPose;
		std::vector<deformer_t> influences;
		FbxTimeSpan * interval;

		anim_t * animations = nullptr;
		bool hasAnim = false;
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

		FbxManager * m_fbxmanager = nullptr;

		FbxScene * m_fbxscene = nullptr;

		bool m_isfbx = false;

	public:
		static int s_LAMBERT_SHADER;

		model_t * m_model;

		Renderer();
		Renderer(std::string model_path);
		Renderer(std::string model_path, bool isfbx);
		Renderer(std::string model_path, std::string material_path);

		void Initialize();

		void SetShader(shader_t * s);
		shader_t * GetShader();

		static void AddModel(model_t * model);

		static void CompileForOpenGL(std::string model_path);
		static void CompileForOpenGLFBX(FbxMesh * mesh, model_t * model);

		static bool IsModelLoaded(std::string model_path);
		
		static bool LoadModel(std::string model_path, std::string material_path);
		bool LoadModelFBX(std::string model_path);
		void ProcessNode(FbxNode * node, FbxNode * parent, model_t * model);
		void ProcessSkeleton(FbxNode * node, FbxNode * parent, skeleton_t * skeleton);
		void ProcessSkinning(FbxMesh * mesh, model_t * model);

		static void FreeModel(std::string model_path);
		static void FreeModelAll();

		void SetModel(std::string model_path, bool isfbx = false);

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