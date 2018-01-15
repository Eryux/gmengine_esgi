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

		float m_yaw = 0.0f;
		float m_pitch = 0.0f;

	public:
		float m_speed = 1.0f;
		float m_sensivity = 0.2f;

		void start();

		void update();

		void SetCameraTransform(Transform * t);

	};

}

#endif