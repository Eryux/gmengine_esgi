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

/*
lm::mat4 Camera::Orientation() const
{
	glm::quaternion rotation(glm::angleAxis(mVerticalAngle, glm::vec3(1.0f, 0.0f, 0.0f)));
	rotation = rotation * glm::angleAxis(mHorizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	return glm::toMat4(rotation);
}

Multiply this with the FPS cameras translation matrix and you have your view matrix.

glm::vec3 mTranslation;

glm::mat4 Camera::GetCameraTransform() const
{
	return Orientation() * glm::translate(glm::mat4(1.0f), -mTranslation);
}
*/


glm::mat4 Camera::GetViewMatrix()
{
	return glm::mat4_cast(m_transform->getLocalRotation()) * glm::translate(glm::mat4(), m_transform->getLocalPosition());
	//return glm::lookAt(m_transform->getLocalPosition(), m_transform->getLocalPosition() + m_transform->getForwardVector() * glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(m_fov, m_ratio, m_near_plane, m_far_plane);
}