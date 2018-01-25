#pragma once

#include <unordered_map>

#include "glm.hpp"
#include "gtc\quaternion.hpp"

#include "Component.h"
#include "Physic.h"

#include "Quat.h"

namespace Engine {

	class Transform : public Component {

	private:
		static std::unordered_map<physx::PxActor *, Transform*> s_actors_transform;

	public:
		glm::vec3 m_localPosition;

		glm::vec3 m_localSize;

		glm::quat m_localRotation;

		Quat m_localRotation_c;

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

		glm::vec3 getRightVector();

		glm::vec3 getUpVector();

		glm::mat4 getWorlMatrix();

		static void setTransform(physx::PxActor * a, physx::PxTransform transform);

		static void addActor(physx::PxActor * a, Transform * t);

		static void removeActor(physx::PxActor * a);

		static void freeActor();

		static Transform * getTransform(physx::PxActor * a);

	};

}