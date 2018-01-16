#define TINYOBJLOADER_IMPLEMENTATION

#include "Renderer.h"
#include "Engine.h"

#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

using namespace Engine;

std::vector<model_t*> Renderer::s_models;

Renderer::Renderer() { 
	m_model_name = "";
	m_material_path = "";
}

Renderer::Renderer(std::string model_path) {
	m_model_name = model_path;
	m_material_path = "../Ressources";
}

Renderer::Renderer(std::string model_path, std::string material_path) {
	m_model_name = model_path;
	m_material_path = material_path;
}

void Renderer::start() {
	m_transform = m_gameObject->getTransform();

	if (m_model_name != "")
		Renderer::LoadModel(m_model_name, m_material_path);

	m_model = GetModel();
	m_core = Core::Get();

	m_core->AddRenderer(this);
}

void Renderer::remove() {
	Core::Get()->RemoveRenderer(this);
}

void Renderer::SetShader(shader_t * s) { m_shader = s; }

shader_t * Renderer::GetShader() { return m_shader; }

bool Renderer::IsModelLoaded(std::string model_path) {
	for (int i = 0; i < Renderer::s_models.size(); i++) {
		if (Renderer::s_models[i]->name == model_path) return true;
	}
	return false;
}

bool Renderer::LoadModel(std::string model_path, std::string material_path) {
	if (Renderer::IsModelLoaded(model_path)) return true;

	model_t * model = new model_t();
	model->data = new tinyobj::attrib_t();

	std::vector<tinyobj::shape_t> * shapes = &(model->shapes);
	std::vector<tinyobj::material_t> * materials = &(model->materials);

	std::string err_load;
	bool r_OBJLoad = tinyobj::LoadObj(model->data, &(model->shapes), &(model->materials), &err_load, model_path.c_str(), material_path.c_str(), true);
	if (r_OBJLoad) {
		model->name = model_path;
		Renderer::s_models.push_back(model);
		Renderer::CompileForOpenGL(model_path);
		model->vbo_ibo_index = Core::Get()->CreateGLBuffer(model->vertices, model->indexes, model->vertices_size, model->indexes_size);
		return true;
	}
	else {
		delete model->data;
		delete model;
		std::cerr << "[ERROR] Renderer - Unable to load model : " << model_path << std::endl;
		std::cerr << err_load << std::endl;
		return false;
	}
}

void Renderer::FreeModel(std::string model_path) {
	model_t * model = nullptr;
	for (int i = 0; i < Renderer::s_models.size(); i++) {
		if (Renderer::s_models[i]->name == model_path) {
			model = Renderer::s_models[i]; break;
		}
	}

	if (model != nullptr) {
		delete model->data;
		delete model->vertices;
		delete model->indexes;
		delete model;
	}

	for (int i = 0; i < Renderer::s_models.size(); i++) {
		if (Renderer::s_models[i] == nullptr) {
			Renderer::s_models.erase(Renderer::s_models.begin() + i);
			i--;
		}
	}
}

void Renderer::FreeModelAll() {
	for (int i = 0; i < Renderer::s_models.size(); i++) {
		model_t * model = Renderer::s_models[i];

		if (model != nullptr) {
			delete model->data;
			delete model->vertices;
			delete model->indexes;
			delete model;
		}
	}

	Renderer::s_models.clear();
}

void Renderer::CompileForOpenGL(std::string model_path) {
	model_t * model = nullptr;
	for (int i = 0; i < Renderer::s_models.size(); i++) {
		if (Renderer::s_models[i]->name == model_path) {
			model = Renderer::s_models[i]; break;
		}
	}

	if (model == nullptr) return;

	std::vector<GLfloat> * vertices = new std::vector<GLfloat>();
	std::vector<GLuint> * indexes = new std::vector<GLuint>();

	for (int i = 0; i < model->shapes.size(); i++) 
	{
		std::vector<tinyobj::index_t> v_indices = model->shapes[i].mesh.indices;
		for (int j = 0; j < v_indices.size(); j++) 
		{
			int vi = v_indices[j].vertex_index * 3;
			int ti = v_indices[j].texcoord_index * 2;
			int ni = v_indices[j].normal_index * 3;

			GLfloat v1 = model->data->vertices[vi];
			GLfloat v2 = model->data->vertices[vi+1];
			GLfloat v3 = model->data->vertices[vi+2];

			GLfloat n1 = model->data->normals[ni];
			GLfloat n2 = model->data->normals[ni+1];
			GLfloat n3 = model->data->normals[ni+2];

			GLfloat t1, t2;
			if (ti >= 0) {
				t1 = model->data->texcoords[ti];
				t2 = model->data->texcoords[ti+1];
			}
			else {
				t1 = 0.f;
				t2 = 1.f;
			}

			vertices->push_back(v1);
			vertices->push_back(v2);
			vertices->push_back(v3);

			vertices->push_back(n1);
			vertices->push_back(n2);
			vertices->push_back(n3);

			vertices->push_back(t1);
			vertices->push_back(t2);

			indexes->push_back(indexes->size());
		}
	}

	model->vertices_size = vertices->size();
	model->vertices = vertices->data();
	model->indexes_size = indexes->size();
	model->indexes = indexes->data();
}

void Renderer::SetModel(std::string model_path) {
	m_model_name = model_path;
	Renderer::LoadModel(m_model_name, m_material_path);
}

void Renderer::SetMaterial(int i) { m_material_id = i; }

tinyobj::material_t * Renderer::GetMaterial(int i) {
	model_t * m = Renderer::GetModel();
	if (m != nullptr) {
		return &(m->materials[i]);
	}
	return nullptr;
}

model_t * Renderer::GetModel() {
	for (int i = 0; i < Renderer::s_models.size(); i++) {
		if (Renderer::s_models[i]->name == m_model_name) {
			return Renderer::s_models[i];
		}
	}
	return nullptr;
}

void Renderer::SetTexture(int i) {
	m_texture_id = i;
}

void Renderer::draw() 
{
	if (m_core == nullptr) return;

	glm::mat4 mat_world = m_core->m_camera->GetViewMatrix() * m_transform->getWorlMatrix();
	glm::mat4 mat_proj = m_core->m_camera->GetProjectionMatrix();

	// SHADER SETTINGS -----------------------------------------

	// Set materials properties
	GLuint program = m_shader->program;
	glUseProgram(program);

	// Default
	auto LightAmbiantColor = glGetUniformLocation(program, "u_lightAmbientColor");
	glUniform3f(LightAmbiantColor, 0.9f, 0.9f, 0.9f);
	auto LightDiffuseColor = glGetUniformLocation(program, "u_lightDiffuseColor");
	glUniform3f(LightDiffuseColor, 1.f, 1.f, 1.f);
	auto materialDiffuseColor = glGetUniformLocation(program, "u_materialDiffuseColor");
	glUniform3f(materialDiffuseColor, 1.f, 1.f, 1.f);
	auto materialAmbiantColor = glGetUniformLocation(program, "u_materialAmbientColor");
	glUniform3f(materialAmbiantColor, 1.f, 1.f, 1.f);
	auto materialTextured = glGetUniformLocation(program, "u_textured");
	glUniform1f(materialTextured, 0.f);

	// Material
	if (m_material_id != -1) 
	{
		tinyobj::material_t * mat = &(m_model->materials[m_material_id]);

		// Texture
		if (m_texture_id != -1)
		{
			uint32_t texUnit = 0;
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, m_core->m_textures[m_texture_id]);

			auto textureLocation = glGetUniformLocation(program, "u_Texture");
			glUniform1i(textureLocation, texUnit);
			glUniform1f(materialTextured, 1.f);
		}

		// Params
		glUniform3fv(materialDiffuseColor, 1, mat->diffuse);
		glUniform3fv(materialAmbiantColor, 1, mat->ambient);
	}

	// Set position
	auto world_location = glGetUniformLocation(program, "u_WorldMatrix");
	glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(mat_world[0]));

	auto projection_location = glGetUniformLocation(program, "u_ProjectionMatrix");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(mat_proj[0]));

	// Set time (required for animation only)
	auto time_location = glGetUniformLocation(program, "u_Time");
	glUniform1f(time_location, 0.f);

	// Set light
	auto lightDir_location = glGetUniformLocation(program, "u_PositionOrDirection");
	float lightVector[4] = { 0.5, -10.0, 0.0, 0.0 };
	glUniform4fv(lightDir_location, 1, lightVector);

	// Set coords
	auto position_location = glGetAttribLocation(program, "a_Position");
	auto texcoords_location = glGetAttribLocation(program, "a_TexCoords");
	auto normal_location = glGetAttribLocation(program, "a_Normal");


	// MESH -------------------------------------------------

	if (m_model->vbo_ibo_index != -1) 
	{
		// Vertex position
		glBindBuffer(GL_ARRAY_BUFFER, m_core->m_vbo[m_model->vbo_ibo_index]);
		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
		glEnableVertexAttribArray(position_location);

		// Normal position
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
		glEnableVertexAttribArray(normal_location);

		// UV position
		glVertexAttribPointer(texcoords_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));
		glEnableVertexAttribArray(texcoords_location);

		// Indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_core->m_ibo[m_model->vbo_ibo_index]);
		glDrawElements(GL_TRIANGLES, m_model->indexes_size, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(position_location);
		glDisableVertexAttribArray(normal_location);
		glDisableVertexAttribArray(texcoords_location);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (m_material_id != -1 && m_texture_id != -1) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glUseProgram(0);
}