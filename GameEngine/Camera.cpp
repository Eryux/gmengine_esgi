#include "Camera.h"
#include "GameObject.h"
#include "SceneLoader.h"

#include "gtc\matrix_transform.hpp"
#include "gtx\quaternion.hpp"
#include "gtx\transform.hpp"

using namespace Engine;

Camera::Camera() {
	Core::Get()->m_camera = this;

	SceneLoader::BindComponent(".PAVCamera@Engine@@");
	SceneLoader::BindParam(".PAVCamera@Engine@@", "m_fov", &m_fov, ".M");
	SceneLoader::BindParam(".PAVCamera@Engine@@", "m_near_plane", &m_near_plane, ".M");
	SceneLoader::BindParam(".PAVCamera@Engine@@", "m_far_plane", &m_far_plane, ".M");
}

void Camera::start() 
{
	sf::RenderWindow * w = Core::Get()->GetWindowContext();
	m_ratio = ((float) w->getSize().x) /  (float) w->getSize().y;
	m_transform = m_gameObject->getTransform();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_transform->getLocalPosition(), m_transform->getLocalPosition() + m_transform->getForwardVector() * 1.f, glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(m_fov, m_ratio, m_near_plane, m_far_plane);
}

glm::vec3 Camera::PointToWorld(glm::vec2 point, glm::vec4 viewport)
{
	return glm::unProject(glm::vec3(point.x, point.y, 0.0f), m_transform->getWorlMatrix() * GetViewMatrix(), GetProjectionMatrix(), viewport);
}