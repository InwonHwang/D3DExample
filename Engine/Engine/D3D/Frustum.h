#pragma once
#include "..\Core\Core.h"

#define PLANE_EPSILON 5.0f

class Frustum
{
public:
	Frustum();
	~Frustum();

	bool Make(const D3DXMATRIXA16& matViewProj);
	bool IsIn(const Vector3& position) const;						// 정점 프러스텀 컬링
	bool IsInSphere(const Vector3& position, float radius) const;	// 경계구 프러스텀 컬링

	void Draw(IDirect3DDevice9& device);								// debug용	

private:
	Vector3* _vertice;
	D3DXPLANE* _planes;
};

