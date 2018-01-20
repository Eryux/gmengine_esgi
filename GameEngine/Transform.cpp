#include "Transform.h"

#include "ext.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtx\quaternion.hpp"
#include "gtx\transform.hpp"

using namespace Engine;

std::unordered_map<physx::PxActor *, Transform*> Transform::s_actors_transform;

Transform::Transform() {
	m_localPosition = { 0.f, 0.f, 0.f };
	m_localSize = { 1.f, 1.f, 1.f };
	m_localRotation = { 0.f, 0.f, 0.f, 1.f };
}

Transform::Transform(glm::vec3 pos) {
	m_localPosition = pos;
	m_localSize = { 1.f, 1.f, 1.f };
	m_localRotation = { 0.f, 0.f, 0.f, 1.f };
}

Transform::Transform(glm::vec3 pos, glm::vec3 size) {
	m_localPosition = pos;
	m_localSize = size;
	m_localRotation = { 0.f, 0.f, 0.f, 1.f };
}

Transform::Transform(glm::vec3 pos, glm::vec3 size, glm::vec3 rot) {
	m_localPosition = pos;
	m_localSize = size;
	m_localRotation = rot;
}

// Position
void Transform::setLocalPosition(glm::vec3 pos) {
	m_localPosition = pos;
}

glm::vec3 Transform::getLocalPosition() {
	return m_localPosition;
}

glm::vec3 Transform::getWorldPosition() {
	return glm::vec3();
}

void Transform::translate(glm::vec3 offset) {
	m_localPosition = glm::translate(glm::mat4(), offset) * glm::vec4(m_localPosition, 0.f);
}

// Rotation
void Transform::setLocalRotation(glm::quat rot) {
	m_localRotation = rot;
}

void Transform::setLocalRotation(glm::vec3 rot) {
	m_localRotation = glm::quat(rot);
}

glm::quat Transform::getLocalRotation() {
	return m_localRotation;
}

glm::vec3 Transform::getLocalEulerRotation() {
	return glm::vec3();
}

glm::quat Transform::getWorldRotation() {
	return glm::quat();
}

glm::vec3 Transform::getWorldEulerRotation() {
	return glm::vec3();
}

// Scale
void Transform::setLocalSize(glm::vec3 size) {
	m_localSize = size;
}

glm::vec3 Transform::getLocalSize() {
	return m_localSize;
}

glm::vec3 Transform::getWorldSize() {
	return glm::vec3();
}

// Others
glm::vec3 Transform::getForwardVector() {
	return m_localRotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Transform::getRightVector() {
	return m_localRotation * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Transform::getUpVector() {
	return m_localRotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::mat4 Transform::getWorlMatrix() {
	glm::mat4 t_mat = glm::translate(glm::mat4(), m_localPosition);
	glm::mat4 s_mat = glm::scale(m_localSize);
	glm::mat4 r_mat = glm::mat4_cast(m_localRotation);
	return t_mat * r_mat * s_mat;
}

void Transform::setTransform(physx::PxActor * a, physx::PxTransform transform)
{
	Transform * t = s_actors_transform[a];
	t->m_localPosition.x = transform.p.x;
	t->m_localPosition.y = transform.p.y;
	t->m_localPosition.z = transform.p.z;
	t->m_localRotation.x = transform.q.x;
	t->m_localRotation.y = transform.q.y;
	t->m_localRotation.z = transform.q.z;
	t->m_localRotation.w = transform.q.w;
}

void Transform::addActor(physx::PxActor * a, Transform * t)
{
	s_actors_transform[a] = t;
}

void Transform::removeActor(physx::PxActor * a)
{
	s_actors_transform.erase(a);
}

Transform * Transform::getTransform(physx::PxActor * a) 
{
	return s_actors_transform[a];
}