#pragma once

#ifndef ENGINE_QUAT_H
#define ENGINE_QUAT_H

#include "glm.hpp"

namespace Engine
{

	class Quat {

	public:
		float x;
		float y;
		float z;
		float w;

		Quat();

		Quat(float angle, glm::vec3 v);

		Quat(float x, float y, float z, float w);

		Quat(const Quat& quat);

		// ------------------------------------------

		float get_norm(glm::vec3 vect);

		float get_norm(Quat quat);

		float radians_to_degrees(float rad);

		float degrees_to_radians(float deg);

		glm::vec3 normalize(glm::vec3 vect);

		Quat normalize(Quat quat);

		glm::mat4 to_mat(Quat * quat);

		Quat conjugate(Quat * quat);

		Quat to_quat(glm::vec3 vect);

	};

}

#endif