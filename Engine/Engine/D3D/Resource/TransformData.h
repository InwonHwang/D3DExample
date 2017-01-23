#pragma once
#include "..\..\Core\Core.h"

class TransformData : public ResourceItem
{
public:
	TransformData(ResourceHandle handle, ResourcePoolImpl* pool);
	~TransformData();

	virtual void Destroy() override {}

private:
	D3DXMATRIX _trasnform;
	D3DXMATRIX _transformParent;
};

