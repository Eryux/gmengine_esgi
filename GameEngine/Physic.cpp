#include "Physic.h"

#include "GameObject.h"

#include <iostream>

using namespace Engine;

physx::PxFoundation * Physic::s_foundation = nullptr;
physx::PxCooking * Physic::s_cooking = nullptr;
physx::PxDefaultAllocator Physic::s_default_allocator;
physx::PxDefaultErrorCallback Physic::s_default_e_callback;
std::vector<physx::PxMaterial*> Physic::s_materials;
PhysicEvent * Physic::s_event_manager = nullptr;
physx::PxPhysics * Physic::s_sdk;
physx::PxScene * Physic::s_scene;

std::vector<collider_t> PhysicEvent::s_colliders;

bool Physic::Init()
{
	s_foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, s_default_allocator, s_default_e_callback);
	s_sdk = PxCreateBasePhysics(PX_PHYSICS_VERSION, *s_foundation, physx::PxTolerancesScale(), false, NULL);

	if (!s_sdk) {
		std::cerr << "Physx SDK init failed !" << std::endl;
		return false;
	}

	if (!PxInitExtensions(*s_sdk, NULL)) {
		std::cerr << "Physx extensions init failes !" << std::endl;
		return false;
	}

	s_cooking = PxCreateCooking(PX_PHYSICS_VERSION, s_sdk->getFoundation(), physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!s_cooking) {
		std::cerr << "Physx cooking init failed !" << std::endl;
		return false;
	}

	// Add default material
	AddMaterial(0.5f, 0.5f, 0.1f);

	return true;
}

void Physic::Free()
{
	s_cooking->release();
	PxCloseExtensions();
	s_sdk->release();
	s_foundation->release();
}


bool Physic::InitScene()
{
	physx::PxSceneDesc scene_desc(s_sdk->getTolerancesScale());
	scene_desc.gravity = physx::PxVec3(0.f, 0.f, 0.f);
	scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;

	physx::PxCpuDispatcher * cpu_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	if (!cpu_dispatcher) {
		std::cerr << "Physx cpu dispatched failed !" << std::endl;
		return false;
	}

	scene_desc.cpuDispatcher = cpu_dispatcher;
	s_scene = s_sdk->createScene(scene_desc);
	if (!s_scene) {
		std::cerr << "Physx create scene failed !" << std::endl;
		return false;
	}
	s_scene->setFlag(physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

	return true;
}


void Physic::FreeScene()
{
	if (s_scene != nullptr)
		s_scene->release();

	PhysicEvent::Free();
}


void Physic::Run(float delta_time)
{
	delta_time += 0.00001f;
	s_scene->simulate(delta_time);
	s_scene->fetchResults(true);

	physx::PxU32 nb_active_actors;
	physx::PxActor** active_actors = s_scene->getActiveActors(nb_active_actors);

	for (physx::PxU32 i = 0; i < nb_active_actors; ++i) {
		Transform * transform = static_cast<Transform*>(active_actors[i]->userData);
		transform->setTransform(active_actors[i], static_cast<physx::PxRigidActor*>(active_actors[i])->getGlobalPose());
	}

	// delete active actor ?
}


unsigned int Physic::AddMaterial(float s_friction, float d_friction, float coeff)
{
	s_materials.push_back(s_sdk->createMaterial(s_friction, d_friction, coeff));
	return s_materials.size() - 1;
}


physx::PxMaterial * Physic::GetMaterial(unsigned int i)
{
	return s_materials[i];
}

// ------------------------------------------------------

GameObject * PhysicEvent::GetColliderGameObject(physx::PxActor * collider)
{
	return Transform::getTransform(collider)->gameObject();
}

void PhysicEvent::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		if (pairs[i].flags & (physx::PxContactPairFlag::eREMOVED_SHAPE_0 | physx::PxContactPairFlag::eREMOVED_SHAPE_1))
			continue;

		GameObject * src_object = GetColliderGameObject(pairHeader.actors[0]);
		if (src_object == nullptr) continue;

		GameObject * target_object = GetColliderGameObject(pairHeader.actors[1]);
		if (target_object == nullptr) continue;

		src_object->onCollision(target_object, pairHeader.actors[1]);
	}
}

void PhysicEvent::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		GameObject * src_object = GetColliderGameObject(pairs[i].triggerActor);
		if (src_object == nullptr) continue;

		GameObject * target_object = GetColliderGameObject(pairs[i].otherActor);
		if (target_object == nullptr) continue;

		src_object->onTrigger(target_object, pairs[i].otherActor);
	}
}

void PhysicEvent::AddPhysicObject(physx::PxActor * actor, GameObject * gameobject)
{
	collider_t collider;
	collider.actor = actor;
	collider.gameobject = gameobject;
	s_colliders.push_back(collider);

	Transform::addActor(actor, gameobject->getTransform());
}

void PhysicEvent::RemovePhysicObject(GameObject * gameobject)
{
	int index = -1;
	physx::PxActor * ptr_actor = nullptr;
	for (int i = 0; i < s_colliders.size(); ++i) {
		if (s_colliders[i].gameobject == gameobject)
		{
			ptr_actor = s_colliders[i].actor;
			index = i;
			break;
		}
	}

	if (index > -1) {
		s_colliders.erase(s_colliders.begin() + index);
	}

	if (ptr_actor != nullptr) {
		Transform::removeActor(ptr_actor);
	}
}

void PhysicEvent::Free()
{
	s_colliders.clear();
}
