#pragma once

#include "Vector.h"

inline void Vector3::Lerp(Vector3& outVec, const Vector3& v1, const Vector3& v2, float frame)
{
	D3DXVec3Lerp(&outVec, &v1, &v2, frame);
}

inline float Vector3::GetDistance(const Vector3& v1, const Vector3& v2)
{
	return D3DXVec3Length(&(v2 - v1));
}

inline float Vector3::GetLength(const Vector3& v)
{
	return D3DXVec3Length(&v);
}
