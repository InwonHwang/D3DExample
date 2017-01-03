#pragma once

#include "../Core/Core.h"
#include "Math/Math.h"

#define PLANE_EPSILON 5.0f

class Transform;

class Frustum
{
public:
	Frustum();
	~Frustum();
		
	bool create();
	void destroy();

	bool make(const D3DXMATRIXA16& m);
	bool isIn(const Vector3& position) const;						// 정점
	bool isInSphere(const Vector3& position, float radius) const;	// 경계구

	// debug용
	void setTransform(Transform& t);
	void draw();

	//임시용
	Vector3 getPos() const { return _camPos; }

private:	
	Vector3* _vertice;
	D3DXPLANE* _planes;
	D3DXMATRIX* _matProj;

	// 임시용
	Transform* _t;
	Vector3 _camPos;
};

