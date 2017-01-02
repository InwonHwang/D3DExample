#pragma once

#include "../Math/Math.h"
#include "../../Core/Core.h"

class Frustum;

// �ӽÿ�
class Transform;

class Camera
{
public:
	enum CameraOption {
		PERSPECTIVE, // perspective or orthographic;
		FRUSTUM
	};

public:
	Camera();
	~Camera();

	virtual void create(flags32 option);
	virtual void destroy();

	virtual void update();

	void flush();

	Frustum* getFrustum() { return _frustum; }

	//�ӽÿ�
	void setTransform(Transform& t)
	{
		_t = &t;
	}
private:
	Frustum* _frustum;
	flags32 _option;

	//�ӽÿ�
	Transform* _t;
};

