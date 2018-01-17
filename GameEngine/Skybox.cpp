#include "Skybox.h"

#include "gtx\transform.hpp"
#include "gtc\type_ptr.hpp"

using namespace Engine;

Skybox::Skybox()
{
	float points[] = {
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_core = Core::Get();
}

bool Skybox::loadTextureSide(GLenum side, std::string tex_file)
{
	sf::Image tex_image = sf::Image();

	if (!tex_image.loadFromFile(tex_file)) {
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);
	glTexImage2D(side, 0, GL_RGBA, tex_image.getSize().x, tex_image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	return true;
}

bool Skybox::loadTexture(std::string tex_path)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_tex);

	loadTextureSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, tex_path + "/front.png");
	loadTextureSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, tex_path + "/back.png");
	loadTextureSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, tex_path + "/top.png");
	loadTextureSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, tex_path + "/bottom.png");
	loadTextureSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, tex_path + "/left.png");
	loadTextureSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, tex_path + "/right.png");

	return true;
}

void Skybox::setShader(uint32_t program) {
	m_render_program = program;
}

void Skybox::draw()
{
	if (m_core == nullptr) return;

	glm::mat4 mat_world = m_core->m_camera->GetViewMatrix() * glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 mat_proj = m_core->m_camera->GetProjectionMatrix();

	glDepthMask(GL_FALSE);
	glUseProgram(m_render_program);

	auto world_location = glGetUniformLocation(m_render_program, "V");
	glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(mat_world[0]));
	auto projection_location = glGetUniformLocation(m_render_program, "P");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(mat_proj[0]));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}

Skybox::~Skybox() 
{
	glDeleteBuffers(1, &m_vbo);
}