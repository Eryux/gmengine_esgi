#pragma once

#ifndef ENGINE_GAMEOBJECT_H
#define ENGINE_GAMEOBJECT_H

#include <vector>

#include "Transform.h"

namespace Engine {

	class Component;

	enum GameObjectState { ENABLE, DISABLE, PENDING_DESTROY };

	class GameObject {
		
	private:
		std::vector<Component> m_components;

		std::string m_objectName;

		GameObjectState m_state;

		GameObject* m_parentObject;

		Transform* m_transform;

	public:
		GameObject();

		template<typename T>
		T* getComponent(); 

		template<typename T>
		std::vector<T*> getComponents();

		std::vector<Component> getComponents();

		void addComponent(Component component);

		void removeComponent(Component* component);

		void removeComponent(int index);

		std::string getName();

		GameObjectState getState();

		void setActive(bool active);

		GameObject* getParent();

		Transform* getTransform();

		void destroy();

		~GameObject();

	};

}

#endif