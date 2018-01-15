#pragma once

#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

namespace Engine {

	class GameObject;

	enum ComponentState { INIT, ENABLED, DISABLED, PENDING_REMOVE };

	class Component {

	protected:
		GameObject* m_gameObject;

		ComponentState m_state;

	public:
		// Call when object is instanciated
		Component();

		// Call at the end of a frame when component is tagged for pending removing
		~Component();

		void setActive(bool status);

		ComponentState getState();

		GameObject* gameObject();

		void setGameObject(GameObject* object);

		// Call in the beginning of game loop when component is enabled
		virtual void start() {}

		// Call every frame of game loop
		virtual void update() {}
		
		// Call when component is enable (in current frame)
		virtual void onEnable() {}

		// Call when component is disabled (in current frame)
		virtual void onDisable() {}

		// Call it for removing component
		virtual void remove() {}
	};

};

#endif