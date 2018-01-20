#include "RigidBody.h"
#include "SceneLoader.h"

using namespace Engine;

RigidBody::RigidBody()
{
	std::string raw_component_name = typeid(RigidBody*).raw_name();

	SceneLoader::BindComponent(raw_component_name);
	SceneLoader::BindParam(raw_component_name, "m_mass", &m_param_mass, ".M");
	SceneLoader::BindParam(raw_component_name, "m_static", &m_param_static, "._N");
	SceneLoader::BindParam(raw_component_name, "m_kinematic", &m_param_kinematic, "._N");
	SceneLoader::BindParam(raw_component_name, "m_trigger", &m_param_trigger, "._N");
	SceneLoader::BindParam(raw_component_name, "m_shape", &m_param_shape, ".I");
	SceneLoader::BindParam(raw_component_name, "m_material", &m_param_material, ".I");
	SceneLoader::BindParam(raw_component_name, "m_shape_param", &m_param_shape_param, ".PAM");
	SceneLoader::BindParam(raw_component_name, "m_constraint_pos", &m_constraint_position_matrix, ".PA_N");
	SceneLoader::BindParam(raw_component_name, "m_constraint_rot", &m_constraint_rotation_matrix, ".PA_N");

	m_param_shape_param = new float[3]{ 0.f, 0.f, 0.f };
}

RigidBody::~RigidBody()
{
	delete m_param_shape_param;

	Physic::s_scene->removeActor(*m_actor);
	m_actor->release();
}

void RigidBody::start()
{
	m_transform = m_gameObject->getTransform();
	glm::vec3 pos = m_transform->getLocalPosition();
	glm::quat rot = m_transform->getLocalRotation();

	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z), physx::PxQuat(rot.x, rot.y, rot.z, rot.w));;
	
	physx::PxGeometry * geometry;
	if (m_param_shape == 0) { // shape : plane
		geometry = &(physx::PxPlaneGeometry());
	}
	else if (m_param_shape == 1) { // shape : cube
		geometry = &physx::PxBoxGeometry(m_param_shape_param[0], m_param_shape_param[1], m_param_shape_param[2]);
	}
	else if (m_param_shape == 2) { // shape : sphere
		geometry = &(physx::PxSphereGeometry(m_param_shape_param[0]));
	}
	else if (m_param_shape == 3) { // shape : capsule
		geometry = &(physx::PxCapsuleGeometry(m_param_shape_param[0], m_param_shape_param[1]));
	}
	else { // cube shape by default
		geometry = &(physx::PxBoxGeometry(m_param_shape_param[0], m_param_shape_param[1], m_param_shape_param[2]));
	}

	if (isStatic()) 
	{
		physx::PxRigidStatic * rigid = Physic::s_sdk->createRigidStatic(transform);
		physx::PxShape * shape = physx::PxRigidActorExt::createExclusiveShape(*rigid, *geometry, *Physic::GetMaterial(m_param_material));
		
		if (isTrigger()) {
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		m_actor = static_cast<physx::PxActor*>(rigid);
	}
	else {
		physx::PxReal density = 1.f;
		m_rigidbody = physx::PxCreateDynamic(*Physic::s_sdk, transform, *geometry, *Physic::GetMaterial(m_param_material), density);
		m_rigidbody->setMass(m_param_mass);

		if (isTrigger()) {
			physx::PxU32 nb_shape = m_rigidbody->getNbShapes();
			physx::PxShape** shapes = new physx::PxShape*[nb_shape];
			m_rigidbody->getShapes(shapes, nb_shape);

			while (nb_shape--) {
				shapes[nb_shape]->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				shapes[nb_shape]->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			}
			
			delete[] shapes;
		}

		m_actor = static_cast<physx::PxActor*>(m_rigidbody);
	}

	Physic::s_scene->addActor(*m_actor);
	PhysicEvent::AddPhysicObject(m_actor, m_gameObject);
	Transform::addActor(m_actor, m_transform);
}

physx::PxActor * RigidBody::getActor()
{
	return m_actor;
}

physx::PxRigidBody * RigidBody::getRigidbody()
{
	return m_rigidbody;
}

bool RigidBody::isKinematic()
{
	return m_param_kinematic;
}

void RigidBody::setKinematic(bool value)
{
	if (!isStatic())
	{
		m_rigidbody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
		m_param_kinematic = value;
	}
}

bool RigidBody::isStatic()
{
	return m_param_static;
}

bool RigidBody::isTrigger()
{
	return m_param_trigger;
}

bool * RigidBody::getConstraintPosition()
{
	return m_constraint_position_matrix;
}

bool * RigidBody::getConstraintRotation()
{
	return m_constraint_rotation_matrix;
}

void RigidBody::setConstraintPosition(bool x, bool y, bool z)
{
	m_constraint_position_matrix[0] = x;
	m_constraint_position_matrix[1] = y;
	m_constraint_position_matrix[2] = z;
	refreshPhysicLock();
}

void RigidBody::setConstraintRotation(bool x, bool y, bool z)
{
	m_constraint_rotation_matrix[0] = x;
	m_constraint_rotation_matrix[1] = y;
	m_constraint_rotation_matrix[2] = z;
	refreshPhysicLock();
}

void RigidBody::refreshPhysicLock() 
{
	physx::PxRigidDynamic * dynamic_rigidbody = dynamic_cast<physx::PxRigidDynamic*>(m_rigidbody);

	physx::PxRigidDynamicLockFlags lock_flags;
	if (m_constraint_position_matrix[0])
		lock_flags |= physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
	if (m_constraint_position_matrix[1])
		lock_flags |= physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
	if (m_constraint_position_matrix[2])
		lock_flags |= physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;

	if (m_constraint_rotation_matrix[0])
		lock_flags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
	if (m_constraint_rotation_matrix[1])
		lock_flags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
	if (m_constraint_rotation_matrix[2])
		lock_flags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;

	dynamic_rigidbody->setRigidDynamicLockFlags(lock_flags);
}

void RigidBody::setPhysicPosition(glm::vec3 pos)
{
	if (!isStatic()) {
		m_rigidbody->getGlobalPose().p = physx::PxVec3(pos.x, pos.y, pos.z);
	}
}

void RigidBody::setPhysicRotation(glm::quat quat)
{
	if (!isStatic()) {
		m_rigidbody->getGlobalPose().q = physx::PxQuat(quat.x, quat.y, quat.z, quat.x);
	}
}