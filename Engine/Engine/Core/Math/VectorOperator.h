#pragma once
#include "Vector.h"

inline Vector2 operator * (float f, const Vector2& v2)
{
	return Vector2(v2.x * f, v2.y *f);
}

inline Vector2& Vector2::operator += (const Vector2& v2)
{
	x += v2.x;
	y += v2.y;
	return (*this);
}

inline Vector2& Vector2::operator -= (const Vector2& v2)
{
	x -= v2.x;
	y -= v2.y;
	return (*this);
}

inline Vector2& Vector2::operator *= (float f)
{
	x *= f;
	y *= f;
	return (*this);
}

inline Vector2& Vector2::operator /= (float f)
{
	x /= f;
	y /= f;
	return (*this);
}

inline Vector2 Vector2::operator + (const Vector2& v2) const
{
	return Vector2(x + v2.x, y + v2.y);
}

inline Vector2 Vector2::operator - (const Vector2& v2) const
{
	return Vector2(x - v2.x, y - v2.y);
}

inline Vector2 Vector2::operator * (float f) const
{
	return Vector2(x * f, y * f);
}

inline Vector2 Vector2::operator / (float f) const
{
	return Vector2(x / f, y / f);
}
inline bool Vector2::operator == (const Vector2& v2) const
{
	return (x == v2.x && y == v2.y) ? true : false;
}

inline bool Vector2::operator != (const Vector2& v2) const
{
	return (x != v2.x || y != v2.y) ? true : false;
}

//////////////////////////////////////////////////////////////////////////////

inline Vector3 operator * (float f, const Vector3& v3)
{
	return Vector3(v3.x * f, v3.y * f, v3.z * f);
}

inline Vector3& Vector3::operator += (const Vector3& v3)
{
	x += v3.x;
	y += v3.y;
	z += v3.z;
	return (*this);
}

inline Vector3& Vector3::operator -= (const Vector3& v3)
{
	x -= v3.x;
	y -= v3.y;
	z -= v3.z;
	return (*this);
}

inline Vector3& Vector3::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	return (*this);
}

inline Vector3& Vector3::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
	return (*this);
}

inline Vector3 Vector3::operator + (const Vector3& v3) const
{
	return Vector3(x + v3.x, y + v3.y, z + v3.z);
}

inline Vector3 Vector3::operator - (const Vector3& v3) const
{
	return Vector3(x - v3.x, y - v3.y, z - v3.z);
}

inline Vector3 Vector3::operator * (float f) const
{
	return Vector3(x * f, y * f, z * f);
}

inline Vector3 Vector3::operator / (float f) const
{
	return Vector3(x / f, y / f, z / f);
}
inline bool Vector3::operator == (const Vector3& v3) const
{
	return (x == v3.x && y == v3.y && z == v3.z) ? true : false;
}

inline bool Vector3::operator != (const Vector3& v3) const
{
	return (x != v3.x || y != v3.y || z != v3.z) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////////////////

inline Vector4 operator * (float f, const Vector4& v4)
{
	return Vector4(v4.x * f, v4.y * f, v4.z * f, v4.w * f);
}

inline Vector4& Vector4::operator += (const Vector4& v4)
{
	x += v4.x;
	y += v4.y;
	z += v4.z;
	w += v4.w;
	return (*this);
}

inline Vector4& Vector4::operator -= (const Vector4& v4)
{
	x -= v4.x;
	y -= v4.y;
	z -= v4.z;
	w -= v4.w;
	return (*this);
}

inline Vector4& Vector4::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return (*this);
}

inline Vector4& Vector4::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return (*this);
}

inline Vector4 Vector4::operator + (const Vector4& v4) const
{
	return Vector4(x + v4.x, y + v4.y, z + v4.z, w + v4.w);
}

inline Vector4 Vector4::operator - (const Vector4& v4) const
{
	return Vector4(x - v4.x, y - v4.y, z - v4.z, w + v4.w);
}

inline Vector4 Vector4::operator * (float f) const
{
	return Vector4(x * f, y * f, z * f, w * f);;
}

inline Vector4 Vector4::operator / (float f) const
{
	return Vector4(x / f, y / f, z / f, w / f);
}
inline bool Vector4::operator == (const Vector4& v4) const
{
	return (x == v4.x && y == v4.y && z == v4.z && w == v4.w) ? true : false;
}

inline bool Vector4::operator != (const Vector4& v4) const
{
	return (x != v4.x || y != v4.y || z != v4.z || w != v4.w) ? true : false;
}