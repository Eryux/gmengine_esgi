#include "Camera.h"
#include "GameObject.h"

#include "gtc\matrix_transform.hpp"
#include "gtx\quaternion.hpp"
#include "gtx\transform.hpp"

using namespace Engine;

void Camera::start() 
{
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