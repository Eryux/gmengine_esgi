#pragma once

#ifndef ENGINE_GAMEOBJECT_H
#define ENGINE_GAMEOBJECT_H

#include <PxPhysicsAPI.h>
#include <vector>

#include "Transform.h"

namespace Engine {

	class Component;

	enum GameObjectState { ENABLE, DISABLE, PENDING_DESTROY };

	class GameObject {
		
	private:
		std::vector<Component*> m_components;


		GameObjectState m_state = ENABLE;

		GameObject* m_parentObject;

		Transform* m_transform;

	public:
		std::string m_objectName;

		GameObject(std::string name);

		template<typename T>
		T* getComponent() {
			for (int i = 0; i < m_components.size(); i++) {
				if (dynamic_cast<T*>(m_components[i]) != nullptr) {
					return dynamic_cast<T*>(m_components[i]);
				}
			}

			return nullptr;
		}

		template<typename T>
		std::vector<T*> getComponents()
		{
			std::vector<T*> results;

			for (int i = 0; i < m_components.size(); i++) {
				if (dynamic_cast<T*>(m_components[i]) != nullptr) {
					results.push_back(m_components[i]);
				}
			}

			return results;
		}

		std::vector<Component*> getComponents();

		void addComponent(Component * component);

		void removeComponent(Component * component);

		void removeComponent(int index);

		std::string getName();

		GameObjectState getState();

		void setActive(bool active);

		GameObject* getParent();

		Transform* getTransform();

		void onCollision(GameObject * object, physx::PxActor * collider);

		void onTrigger(GameObject * object, physx::PxActor * collider);

		void destroy();

		~GameObject();

	};

}

#endif