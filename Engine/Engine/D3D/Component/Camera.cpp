#include "Camera.h"
#include "D3DComponent.h"


Camera::Camera()
{
	_transform.reset();
	_matView = new D3DXMATRIX();
	_matProj = new D3DXMATRIX();
	D3DXMatrixIdentity(_matView);
	D3DXMatrixIdentity(_matProj);
}


Camera::~Camera()
{		
}

void Camera::Destroy()
{
	_transform.reset();
	SafeDelete<D3DXMATRIX>(_matView);
	SafeDelete<D3DXMATRIX>(_matProj);
}

void Camera::Update(IDirect3DDevice9& device)
{	
	device.SetTransform(D3DTS_VIEW, _matView);
	device.SetTransform(D3DTS_PROJECTION, _matProj);
}

void Camera::UpdateViewMatrix()
{
	if (_transform.expired()) return;

	sp<Transform> transform = _transform.lock();

	Vector3 eye;
	Vector3 lookAt;
	Vector3 up;
	Vector3 axisY;
	Vector3 axisZ(0, 0, 0);	

	transform->GetLocalPosition(eye);
	transform->GetAxisY(axisY);
	transform->GetAxisZ(axisZ);

	lookAt = eye + axisZ;
	up = axisY;


	D3DXMatrixLookAtLH(_matView, &eye, &lookAt, &up);
	D3DXMatrixPerspectiveFovLH(_matProj, D3DXToRadian(45), 1.0f, 1.0f, 5000.0f);

	//D3DXMatrixOrthoLH(_matProj, 1, 0.5f, 0.5f, 1000.0f);
}

void Camera::SetTransform(sp<Transform> transform)
{
	_transform = transform;
}
