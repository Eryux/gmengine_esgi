#include "GameObject.h"

using namespace Engine;

GameObject::GameObject(std::string name)
{
	m_objectName = name;
	m_transform = new Transform();
}

std::vector<Component*> GameObject::getComponents()
{
	return m_components;
}

void GameObject::addComponent(Component * component)
{
	component->setGameObject(this);

	if (m_components.capacity() < m_components.size() + 8) {
		m_components.reserve(m_components.capacity() + 16);
	}

	m_components.push_back(component);
}

void GameObject::removeComponent(Component* component)
{
	for (int i = 0; i < m_components.size(); i++) {
		if (m_components[i] == component) {
			m_components[i]->remove();
			delete m_components[i];
			m_components.erase(m_components.begin() + i);
			break;
		}
	}
}

void GameObject::removeComponent(int index)
{
	m_components[index]->remove();
	delete m_components[index];
	m_components.erase(m_components.begin() + index);
}

std::string GameObject::getName()
{
	return m_objectName;
}

GameObjectState GameObject::getState()
{
	return m_state;
}

void GameObject::setActive(bool active)
{
	if (m_state == GameObjectState::PENDING_DESTROY) {
		return;
	}

	for (int i = 0; i < m_components.size(); i++) {
		m_components[i]->setActive(active);
	}

	m_state = (active) ? GameObjectState::ENABLE : GameObjectState::DISABLE;
}

GameObject* GameObject::getParent()
{
	return m_parentObject;
}

Transform* GameObject::getTransform() 
{
	return m_transform;
}

void GameObject::destroy()
{
	for (int i = 0; i < m_components.size(); i++) {
		m_components[i]->remove();
		delete m_components[i];
	}

	m_state = GameObjectState::PENDING_DESTROY;
}

GameObject::~GameObject()
{
	delete m_transform;
}
