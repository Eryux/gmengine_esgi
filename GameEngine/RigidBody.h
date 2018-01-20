#pragma once

#ifndef ENGINE_RIGIDBODY_H
#define ENGINE_RIGIDBODY_H

#include "Physic.h"
#include "Engine.h"

namespace Engine {

	class RigidBody : public Component {

	private:
		physx::PxRigidBody * m_rigidbody = nullptr;

		physx::PxActor * m_actor;

		Transform * m_transform;

		float m_param_mass = 1.0f;

		bool m_param_static = false;

		bool m_param_kinematic = false;

		bool m_param_trigger = false;

		unsigned int m_param_material = 0;

		unsigned int m_param_shape = 0; // 0 = plane, 1 = cube, 2 = sphere, 3 = capsule

		float * m_param_shape_param;

		bool m_constraint_position_matrix[3] = { false, false, false };

		bool m_constraint_rotation_matrix[3] = { false, false, false };

		void refreshPhysicLock();

	public:

		RigidBody();

		~RigidBody();

		void start();

		physx::PxActor * getActor();

		physx::PxRigidBody * getRigidbody();

		bool isKinematic();

		void setKinematic(bool value);

		bool isStatic();

		bool isTrigger();

		bool * getConstraintPosition();

		bool * getConstraintRotation();

		void setConstraintPosition(bool x, bool y, bool z);

		void setConstraintRotation(bool x, bool y, bool z);

		void setPhysicPosition(glm::vec3 pos);

		void setPhysicRotation(glm::quat quat);

	};

}

#endif