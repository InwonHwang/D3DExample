#pragma once
#include <d3dx9.h>

class Matrix : public D3DXMATRIX
{
public:
	Matrix() {}
	Matrix(const Matrix& Src) : D3DXMATRIX(Src) {}
	Matrix(const D3DXMATRIX& Src) : D3DXMATRIX(Src) {}
	Matrix(const float* floats) : D3DXMATRIX(floats) {}	
	Matrix(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44)
		: D3DXMATRIX(_11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34,
					_41, _42, _43, _44) {}
	
};
