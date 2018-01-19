#pragma once

#ifndef MOVE_CAMERA_COMPONENT_H
#define MOVE_CAMERA_COMPONENT_H

#include "Engine.h"
#include "Component.h"
#include "Transform.h"
#include "Input.h"

namespace Engine {

	class MoveCamera_Component : public Component {


	private:
		Transform * m_cameraTransform = nullptr;

		Core * m_core;

		Camera * m_camera;

		float m_yaw = 0.0f;
		float m_pitch = 0.0f;

		bool m_lock_cursor = true;

	public:
		float m_speed = 1.0f;
		float m_sensivity = 0.1f;

		MoveCamera_Component();

		void start();

		void update();

		void SetCameraTransform(Transform * t);

	};

}

#endif