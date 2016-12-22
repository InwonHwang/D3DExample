#pragma once
#include "d3dx9math.h"

class Vector4 : public D3DXVECTOR4
{
public:
	Vector4() {}
	Vector4(const Vector4& v) : D3DXVECTOR4(v) {};
	Vector4(const D3DXVECTOR4& v) : D3DXVECTOR4(v) {};
	Vector4(float x, float y, float z, float w) : D3DXVECTOR4(x, y, z, w) {};
	~Vector4() {}
};
