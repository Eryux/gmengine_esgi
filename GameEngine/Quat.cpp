#include "Quat.h"

using namespace Engine;

#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923

Quat::Quat()
{
	x = 0.f; y = 0.f; z = 0.f; w = 0.f;
}

Quat::Quat(float angle, glm::vec3 v)
{
	float sin_a = sin(angle / 2);

	w = cos(angle / 2);
	x = v.x * sin_a;
	y = v.y * sin_a;
	z = v.z * sin_a;
}

Quat::Quat(float x, float y, float z, float w)
{
	x = x; y = y; z = z; w = w;
}

Quat::Quat(const Quat& quat)
{
	x = quat.x; y = quat.y; z = quat.z; w = quat.w;
}

// ------------------------------------------

float Quat::get_norm(glm::vec3 v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Quat::get_norm(Quat q)
{
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

float Quat::radians_to_degrees(float rad)
{
	return rad * 180 / M_PI;
}

float Quat::degrees_to_radians(float deg)
{
	return deg * M_PI / 180;
}

glm::vec3 Quat::normalize(glm::vec3 v)
{
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x = v.x / length;
	v.y = v.y / length;
	v.z = v.z / length;
	return v;
}

Quat Quat::normalize(Quat quat)
{
	Quat norm;
	float length = get_norm(quat);
	norm.x = quat.x / length;
	norm.y = quat.y / length;
	norm.z = quat.z / length;
	norm.w = quat.w / length;
	return norm;
}

glm::mat4 Quat::to_mat(Quat * quat)
{
	glm::mat4 r;

	float xx = quat->x * quat->x;
	float xy = quat->x * quat->y;
	float xz = quat->x * quat->z;
	float xw = quat->x * quat->w;

	float yy = quat->y * quat->y;
	float yz = quat->y * quat->z;
	float yw = quat->y * quat->w;

	float zz = quat->z * quat->z;
	float zw = quat->z * quat->w;

	r[0][0] = 1 - 2 * yy - 2 * zz;
	r[0][1] = 2 * xy - 2 * zw;
	r[0][2] = 2 * xz + 2 * yw;

	r[1][0] = 2 * xy + 2 * zw;
	r[1][1] = 1 - 2 * xx - 2 * zz;
	r[1][2] = 2 * yz - 2 * xw;

	r[2][0] = 2 * xz - 2 * yw;
	r[2][1] = 2 * yz + 2 * xw;
	r[2][2] = 1 - 2 * xx - 2 * yy;

	r[3][3] = 1.f;
	return r;
}

Quat Quat::conjugate(Quat * quat)
{
	return Quat(-quat->x, -quat->y, -quat->z, quat->w);
}

Quat Quat::to_quat(glm::vec3 vect)
{
	return Quat(0, vect.x, vect.y, vect.z);
}