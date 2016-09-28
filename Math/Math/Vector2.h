#pragma once
#include <d3dx9.h>

class Vector2 : public D3DXVECTOR2
{
public:
	Vector2() {}
	Vector2(const Vector2& v) : D3DXVECTOR2(v) {};
	Vector2(const D3DXVECTOR2& v) : D3DXVECTOR2(v) {};
	Vector2(float x, float y) : D3DXVECTOR2(x, y) {};
	~Vector2() {}
};
