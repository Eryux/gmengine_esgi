#pragma once

#include "glm.hpp"
#include "gtc\quaternion.hpp"

#include "Component.h"

namespace Engine {

	class Transform : public Component {

	private:
		glm::vec3 m_localPosition;

		glm::vec3 m_localSize;

		glm::quat m_localRotation;

	public:
		Transform();

		Transform(glm::vec3 pos);

		Transform(glm::vec3 pos, glm::vec3 size);

		Transform(glm::vec3 pos, glm::vec3 size, glm::vec3 rot);

		// Position
		void setLocalPosition(glm::vec3 pos);

		glm::vec3 getLocalPosition();

		glm::vec3 getWorldPosition();

		void translate(glm::vec3 offset);

		// Rotation
		void setLocalRotation(glm::quat rot);

		void setLocalRotation(glm::vec3 rot);

		glm::quat getLocalRotation();

		glm::vec3 getLocalEulerRotation();

		glm::quat getWorldRotation();

		glm::vec3 getWorldEulerRotation();

		// Scale
		void setLocalSize(glm::vec3 size);

		glm::vec3 getLocalSize();

		glm::vec3 getWorldSize();

		// Others
		glm::vec3 getForwardVector();

		glm::mat4 getWorlMatrix();

	};

}