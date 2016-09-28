#pragma once
#include <d3dx9.h>

class Vector3 : public D3DXVECTOR3
{
public:
	Vector3() {}
	Vector3(const Vector3& v) : D3DXVECTOR3(v) {};
	Vector3(const D3DXVECTOR3& v) : D3DXVECTOR3(v) {};
	Vector3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {};
	~Vector3() {}
};

