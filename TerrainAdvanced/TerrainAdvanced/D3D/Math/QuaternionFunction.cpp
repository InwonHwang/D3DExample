#include "Quaternion.h"
#include "Mathf.h"
#include "Vector.h"
#include "../../Core/Core.h"

//http://stackoverflow.com/questions/11492299/quaternion-to-euler-angles-algorithm-how-to-convert-to-y-up-and-between-ha/11505219 참조

Quaternion Quaternion::Euler(float x, float y, float z)
{
	float t0 = Mathf::Sin(z * 0.5f);
	float t1 = Mathf::Cos(z * 0.5f);
	float t2 = Mathf::Sin(x * 0.5f);
	float t3 = Mathf::Cos(x * 0.5f);
	float t4 = Mathf::Sin(y * 0.5f);
	float t5 = Mathf::Cos(y * 0.5f);
	Quaternion result;
	result.x = t5 * t2 * t1 + t4 * t3 * t0;
	result.y = t4 * t3 * t1 - t5 * t2 * t0;
	result.z = t5 * t3 * t0 - t4 * t2 * t1;
	result.w = t5 * t3 * t1 + t4 * t2 * t0;
	return result;
}

Vector3 Quaternion::ToEulerAngle(const Quaternion& q)
{
	Vector3 v;	

	v.x = Mathf::Atan2(2 * q.x*q.w - 2 * q.y*q.z, 1 - 2 * q.x*q.x - 2 * q.z*q.z) * Mathf::RadToDeg();
	v.y = Mathf::Atan2(2 * q.y*q.w - 2 * q.x*q.z, 1 - 2 * q.y*q.y - 2 * q.z*q.z) * Mathf::RadToDeg();
	v.z = Mathf::Asin(2 * q.x*q.y + 2 * q.z*q.w) * Mathf::RadToDeg();

	return v;
}


// http://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space 참조

Vector3 Quaternion::Rotate(const Vector3& v, float x, float y, float z)
{
	Vector3 ret = v;

	ret.x = ret.x * Mathf::Cos(z) - ret.y * Mathf::Sin(z);
	ret.y = ret.x * Mathf::Sin(z) + ret.y * Mathf::Cos(z);
	ret.z = ret.z;

	ret.x = ret.x * Mathf::Cos(y) + ret.z * Mathf::Sin(y);
	ret.z = ret.x * -Mathf::Sin(y) + ret.z * Mathf::Cos(y);

	ret.y = ret.y * Mathf::Cos(x) - ret.z * Mathf::Sin(x);
	ret.z = ret.y * Mathf::Sin(x) + ret.z * Mathf::Cos(x);

	return ret;
}