#pragma once

#if _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PxTaskDEBUG_x86.lib")
#pragma comment(lib, "PxFoundationDEBUG_x86.lib")
#pragma comment(lib, "PxPvdSDKDEBUG_x86.lib")
#pragma comment(lib, "SceneQueryDEBUG.lib")
#pragma comment(lib, "SimulationControllerDEBUG.lib")
#else
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysX3Cooking_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PxTask_x86.lib")
#pragma comment(lib, "PxFoundation_x86.lib")
#pragma comment(lib, "PxPvdSDK_x86.lib")
#pragma comment(lib, "SceneQuery.lib")
#endif

#ifndef ENGINE_PHYSIC_H
#define ENGINE_PHYSIC_H

#include <PxPhysicsAPI.h>
#include <extensions\PxDefaultAllocator.h>
#include <extensions\PxDefaultErrorCallback.h>
#include <extensions\PxExtensionsAPI.h>

#include <vector>

namespace Engine {

	class GameObject;

	struct {
		physx::PxActor * actor;
		GameObject * gameobject;
	} typedef collider_t;

	class EngineActor : public physx::PxActor {

	public:
		GameObject * m_gameobject = nullptr;

	};

	class EngineStaticActor : physx::PxRigidStatic {
	public:
		EngineActor * m_actor = nullptr;
	};

	class EngineDynamicActor : physx::PxRigidDynamic {
	public:
		EngineActor * m_actor = nullptr;
	};

	class PhysicEvent : public physx::PxSimulationEventCallback {

	private:

		static std::vector<collider_t> s_colliders;

	public:

		void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 count);

		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

		static void AddPhysicObject(physx::PxActor * actor, GameObject * gameobject);

		static void RemovePhysicObject(GameObject * gameobject);

		static GameObject * GetColliderGameObject(physx::PxActor * collider);

		static void Free();

	};

	class Physic {

	private:

		static physx::PxFoundation * s_foundation;

		static physx::PxCooking * s_cooking;

		static physx::PxDefaultAllocator s_default_allocator;

		static physx::PxDefaultErrorCallback s_default_e_callback;

		static std::vector<physx::PxMaterial*> s_materials;

		static PhysicEvent * s_event_manager;

	public:
		static physx::PxPhysics * s_sdk;

		static physx::PxScene * s_scene;

		static bool Init();

		static void Free();

		static bool InitScene();

		static void FreeScene();

		static void Run(float delta_time);

		static unsigned int AddMaterial(float s_friction, float d_friction, float coeff);

		static physx::PxMaterial * GetMaterial(unsigned int);

		static bool raycast(physx::PxVec3 origin, physx::PxVec3 direction, float dist, physx::PxRaycastBuffer * hit);

	};

}

#endif