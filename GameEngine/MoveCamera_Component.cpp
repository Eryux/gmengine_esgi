#include "MoveCamera_Component.h"
#include "SceneLoader.h"

#include <SFML/Window.hpp>

#include <iostream>

using namespace Engine;

MoveCamera_Component::MoveCamera_Component() 
{
	SceneLoader::BindComponent(".PAVMoveCamera_Component@Engine@@");
	SceneLoader::BindParam(".PAVMoveCamera_Component@Engine@@", "m_speed", &m_speed, ".M");
	SceneLoader::BindParam(".PAVMoveCamera_Component@Engine@@", "m_camera", &m_camera, ".PAVCamera@Engine@@");
}

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

	/*if (Input::mouseDown(sf::Mouse::Button::Left))
	{
		std::cout << "left mouse down !" << std::endl;

		int * mousePos = Input::mouseAxis();
		auto viewport = m_core->GetWindowContext()->getSize();

		glm::vec3 world_pos = m_camera->PointToWorld(
			glm::vec2(mousePos[0], mousePos[1]),
			glm::vec4(0, 0, viewport.x, viewport.y)
		);

		physx::PxRaycastBuffer hit;

		glm::vec3 dir = m_cameraTransform->getForwardVector();
		physx::PxVec3 direction = physx::PxVec3(dir.x, dir.y, dir.z);
		physx::PxVec3 origin = physx::PxVec3(world_pos.x, world_pos.y, world_pos.z);

		bool ray = Physic::raycast(origin, direction, 100.f, &hit);
		if (ray)
		{
			physx::PxVec3 pos = hit.block.position;
			std::cout << "Hit : ";
			std::cout << "x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;
			

			GameObject * g = new GameObject("cube");

			Transform * t = g->getTransform();
			t->setLocalPosition(glm::vec3(world_pos.x, world_pos.y, world_pos.z));

			Renderer * r = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
			r->SetMaterial(0);
			r->SetShader(m_core->m_shaders.GetShader(0));
			g->addComponent(r);
			m_core->m_gameObjects.push_back(g);

			g = new GameObject("cube");

			t = g->getTransform();
			t->setLocalPosition(glm::vec3(pos.x, pos.y, pos.z));

			r = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
			r->SetMaterial(0);
			r->SetShader(m_core->m_shaders.GetShader(0));
			g->addComponent(r);
			m_core->m_gameObjects.push_back(g);
		}

		std::cout << "x " << world_pos.x << " y " << world_pos.y << " z " << world_pos.z << std::endl;
	}*/
}