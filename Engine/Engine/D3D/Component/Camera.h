#pragma once
#include "..\..\Core\Core.h"
#include "Component.h"

class Transform;

class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void Destroy() override;
		
	void Update(IDirect3DDevice9& device);
	void UpdateViewMatrix();

	D3DXMATRIX GetViewMatrix() const;
	D3DXMATRIX GetProjMatrix() const;

	void SetTransform(sp<Transform> transform);

private:
	wp<Transform> _transform;
	D3DXMATRIX* _matView;
	D3DXMATRIX* _matProj;
};

inline D3DXMATRIX Camera::GetViewMatrix() const
{
	return *_matView;
}

inline D3DXMATRIX Camera::GetProjMatrix() const
{
	return *_matProj;
}

