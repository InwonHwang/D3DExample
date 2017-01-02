#pragma once

#include "../Math/Math.h"
#include "../../Core/Core.h"

class Frustum;

// 임시용
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

	//임시용
	void setTransform(Transform& t)
	{
		_t = &t;
	}
private:
	Frustum* _frustum;
	flags32 _option;

	//임시용
	Transform* _t;
};

