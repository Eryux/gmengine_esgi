#pragma once

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "Component.h"
#include "Transform.h"

namespace Engine {

	class Camera : public Component {

	private:
		Transform * m_transform;

	public:
		float m_fov;
		float m_ratio;
		float m_near_plane;
		float m_far_plane;

		Camera();

		void start();

		glm::mat4 GetViewMatrix();

		glm::mat4 GetProjectionMatrix();

	};

};


#endif