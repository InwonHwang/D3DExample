#include "Camera.h"
#include "Transform.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::init()
{
	memset(&_matView, 0, sizeof(D3DXMATRIX));
}
void Camera::update()
{
	flush();
}

void Camera::flush()
{
	Vector3 lookAt;
	Vector3 up = t->getAxisZ();
	Vector3 eye = t->getPosition();

	D3DXVec3Normalize(&lookAt, &(t->getPosition() - Vector3(0, 0, 1)));
	D3DXMatrixLookAtLH(&_matView, &eye, &lookAt, &up);
}