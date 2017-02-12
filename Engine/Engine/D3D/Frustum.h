#pragma once
#include "..\Core\Core.h"

#define PLANE_EPSILON 5.0f

class Frustum
{
public:
	Frustum();
	~Frustum();

	bool Make(const D3DXMATRIXA16& matViewProj);
	bool IsIn(const Vector3& position) const;						// ���� �������� �ø�
	bool IsInSphere(const Vector3& position, float radius) const;	// ��豸 �������� �ø�

	void Draw(IDirect3DDevice9& device);							// debug��
	Vector3 GetCameraPos() const;
	
private:
	Vector3* _vertice;
	D3DXPLANE* _planes;
};

inline Vector3 Frustum::GetCameraPos() const
{
	return (_vertice[0] + _vertice[5] / 2.0f);
}
