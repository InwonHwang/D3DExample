#pragma once

#include "../Math/Math.h"

class Transform;

class Camera
{


public:
	Camera();
	~Camera();

	virtual void init();
	virtual void update();

	void flush();

	D3DXMATRIX getMatrixView() { return _matView; }

private:
	Transform* t;
	D3DXMATRIX _matView;	
};

