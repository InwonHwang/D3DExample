#pragma once

#include "d3dx9math.h"

class Quaternion : public D3DXQUATERNION
{
public:
	Quaternion() : D3DXQUATERNION() {}
	Quaternion(const Quaternion& q) : D3DXQUATERNION(q) {}
	Quaternion(float x, float y, float z, float w) : D3DXQUATERNION(x, y, z, w) {}
	~Quaternion() {}

	Quaternion& operator += (const Quaternion&);
	Quaternion& operator -= (const Quaternion&);
	Quaternion& operator *= (const Quaternion&);
	Quaternion& operator *= (float);
	Quaternion& operator /= (float);

	Quaternion operator + (const Quaternion&) const;
	Quaternion operator - (const Quaternion&) const;
	Quaternion operator * (const Quaternion&) const;
	Quaternion operator * (float) const;
	Quaternion operator / (float) const;

	friend Quaternion operator * (float, const Quaternion&);

	bool operator == (const Quaternion&) const;
	bool operator != (const Quaternion&) const;
};
