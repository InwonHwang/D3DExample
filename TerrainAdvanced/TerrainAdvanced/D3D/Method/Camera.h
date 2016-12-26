#pragma once

#include "../Math/Math.h"

class Camera
{


public:
	Camera();
	~Camera();

private:
	D3DXMATRIX _matView;
	
};

