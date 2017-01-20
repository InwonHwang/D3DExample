#pragma once
#include "Quaternion.h"

inline Quaternion operator * (float f, const Quaternion &q)
{
	return Quaternion(f * q.x, f * q.y, f * q.z, f * q.w);
}

inline Quaternion& Quaternion::operator += (const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return(*this);
}

inline Quaternion& Quaternion::operator -= (const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return(*this);
}

inline Quaternion& Quaternion::operator *= (const Quaternion& q)
{
	x *= q.x;
	y *= q.y;
	z *= q.z;
	w *= q.w;
	return(*this);
}

inline Quaternion& Quaternion::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return(*this);
}

inline Quaternion& Quaternion::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return(*this);
}

inline Quaternion Quaternion::operator + (const Quaternion& q) const
{
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

inline Quaternion Quaternion::operator - (const Quaternion& q) const
{
	return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

inline Quaternion Quaternion::operator * (const Quaternion& q) const
{
	return Quaternion(x * q.x, y * q.y, z * q.z, w * q.w);
}

inline Quaternion Quaternion::operator * (float f) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

inline Quaternion Quaternion::operator / (float f) const
{
	return Quaternion(x / f, y / f, z / f, w / f);
}



inline bool Quaternion::operator == (const Quaternion& q) const
{
	return (x == q.x && y == q.y && z == q.z && w == q.w) ? true : false;
}

inline bool Quaternion::operator != (const Quaternion& q) const
{
	return (x != q.x || y != q.y || z != q.z || w != q.w) ? true : false;
}
