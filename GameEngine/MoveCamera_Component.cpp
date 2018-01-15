#include "MoveCamera_Component.h"

#include <SFML/Window.hpp>

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
	Input::bindKeyboard(sf::Keyboard::Key::W);

	m_core->GetWindowContext()->setMouseCursorVisible(false);
}

void MoveCamera_Component::SetCameraTransform(Transform * t) {
	m_cameraTransform = t;
}

void MoveCamera_Component::update() 
{
	float * mouseDelta = Input::mouseDelta();
	m_yaw += mouseDelta[0] * m_sensivity * m_core->m_deltaTime;
	m_pitch += mouseDelta[1] * m_sensivity * m_core->m_deltaTime;

	if (m_pitch > 360.f) m_pitch -= 360.f;
	else if (m_pitch < -360.f) m_pitch += 360.f;

	if (m_pitch > 90.f && m_pitch < 270.f || (m_pitch < -90.f && m_pitch > -270.f))
		m_yaw -= mouseDelta[0] * m_sensivity * m_core->m_deltaTime;
	else
		m_yaw += mouseDelta[0] * m_sensivity * m_core->m_deltaTime;

	if (m_yaw > 360.f) m_yaw -= 360.f;
	else if (m_yaw < -360.f) m_yaw += 360.f;

	glm::quat p_quat = glm::quat(std::cos(m_pitch), std::sin(m_pitch), 0.f, 0.f);
	glm::quat y_quat = glm::quat(std::cos(-m_yaw), 0.f, std::sin(-m_yaw), 0.f);
	glm::quat rot = y_quat * p_quat;
	m_cameraTransform->setLocalRotation(rot);

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
		glm::vec3 new_pos = pos + right * (m_speed * 1.5f * m_core->m_deltaTime);
		m_cameraTransform->setLocalPosition(new_pos);
	}
	if (Input::keyPress(sf::Keyboard::Key::D))
	{
		glm::vec3 pos = m_cameraTransform->getLocalPosition();
		glm::vec3 right = m_cameraTransform->getRightVector();
		glm::vec3 new_pos = pos + right * (-m_speed * 1.5f * m_core->m_deltaTime);
		m_cameraTransform->setLocalPosition(new_pos);
	}

	// Lock cursor
	if (Input::keyDown(sf::Keyboard::Key::W)) {
		m_lock_cursor = !m_lock_cursor;
		m_core->GetWindowContext()->setMouseCursorVisible(!m_lock_cursor);
	}

	if (m_lock_cursor) {
		sf::Window * window = m_core->GetWindowContext();
		sf::Vector2u window_size = window->getSize();
		sf::Mouse::setPosition(sf::Vector2i(window_size.x / 2, window_size.y / 2), *window);
		Input::resetMousePosition(window_size.x / 2, window_size.y / 2);
	}
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