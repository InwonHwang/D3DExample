#pragma once

#include "Vector.h"

inline float Vector3::GetDistance(const Vector3& v1, const Vector3& v2)
{
	return D3DXVec3Length(&(v2 - v1));
}
