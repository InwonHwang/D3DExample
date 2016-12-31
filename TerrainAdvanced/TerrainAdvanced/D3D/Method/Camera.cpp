#include "Camera.h"
#include "Transform.h"
#include "../Device.h"
#include "../Frustum.h"

Camera::Camera()
	: _frustum(nullptr)
{
}


Camera::~Camera()
{
}

void Camera::create(flags32 option)
{
	//_option = option;

	//if (option.check(CameraOption::FRUSTUM))
	//{
	//	_frustum = new Frustum();
	//	// projection ÇÔ¼ö¸¦ °öÇØÁà¾ßÇÔ
	//	_frustum->create(&_t->getMatrix());
	//}
}

void  Camera::destroy()
{
	//SAFE_DELETE(_frustum);
}

void Camera::update()
{
	flush();
}

void Camera::flush()
{
	D3DDevice->SetTransform(D3DTS_VIEW, &_t->getMatrix());
	
}