#include "MoveCamera_Component.h"

#include <iostream>

using namespace Engine;

void MoveCamera_Component::start()
{
	m_core = Core::Get();
	m_cameraTransform = m_core->m_camera->gameObject()->getTransform();

	Input::bindKeyboard(sf::Keyboard::Key::Z);
	Input::bindKeyboard(sf::Keyboard::Key::Q);
	Input::bindKeyboard(sf::Keyboard::Key::S);
	Input::bindKeyboard(sf::Keyboard::Key::D);
}

void MoveCamera_Component::SetCameraTransform(Transform * t) {
	m_cameraTransform = t;
}

void MoveCamera_Component::update() 
{
	if (Input::keyPress(sf::Keyboard::Key::Z)) 
	{
		glm::vec3 pos = m_cameraTransform->getLocalPosition();
		glm::vec3 forward = m_cameraTransform->getForwardVector();
		glm::vec3 new_pos = pos + forward * (m_speed * m_core->m_deltaTime);
		m_cameraTransform->setLocalPosition(new_pos);
	}
	if (Input::keyPress(sf::Keyboard::Key::S))
	{
		glm::vec3 pos = m_cameraTransform->getLocalPosition();
		glm::vec3 forward = m_cameraTransform->getForwardVector();
		glm::vec3 new_pos = pos + forward * (-m_speed * m_core->m_deltaTime);
		m_cameraTransform->setLocalPosition(new_pos);
	}
	if (Input::keyPress(sf::Keyboard::Key::Q))
	{
		glm::vec3 pos = m_cameraTransform->getLocalPosition();
		glm::vec3 right = m_cameraTransform->getRightVector();
		glm::vec3 new_pos = pos + right * (-m_speed * 1.5f * m_core->m_deltaTime);
		m_cameraTransform->setLocalPosition(new_pos);
	}
	if (Input::keyPress(sf::Keyboard::Key::D))
	{
		glm::vec3 pos = m_cameraTransform->getLocalPosition();
		glm::vec3 right = m_cameraTransform->getRightVector();
		glm::vec3 new_pos = pos + right * (m_speed * 1.5f * m_core->m_deltaTime);
		m_cameraTransform->setLocalPosition(new_pos);
	}

	float * mouseDelta = Input::mouseDelta();
	m_pitch = mouseDelta[1] * m_sensivity * m_core->m_deltaTime;
	m_yaw = mouseDelta[0] * m_sensivity * m_core->m_deltaTime;

	//glm::quaternion rotation(glm::angleAxis(mVerticalAngle, glm::vec3(1.0f, 0.0f, 0.0f)));
	//rotation = rotation * glm::angleAxis(mHorizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::quat rot = glm::angleAxis(m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	rot = rot * glm::angleAxis(m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	m_cameraTransform->setLocalRotation(rot * m_cameraTransform->getLocalRotation());
	m_cameraTransform->setLocalRotation(glm::normalize(m_cameraTransform->getLocalRotation()));

}

/*
void CameraFPSQuaternion::UpdateView()
{
	//temporary frame quaternion from pitch,yaw,roll 
	//here roll is not used
	glm::quat key_quat = glm::quat(glm::vec3(key_pitch, key_yaw, key_roll));
	//reset values
	key_pitch = key_yaw = key_roll = 0;

	//order matters,update camera_quat
	camera_quat = key_quat * camera_quat;
	camera_quat = glm::normalize(camera_quat);
	glm::mat4 rotate = glm::mat4_cast(camera_quat);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -eyeVector);

	viewMatrix = rotate * translate;
}*/