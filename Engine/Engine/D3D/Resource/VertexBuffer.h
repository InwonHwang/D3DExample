#pragma once
#include "..\..\Core\Core.h"

class VertexBuffer : public ResourceItem
{
public:
	VertexBuffer(ResourceHandle handle, ResourcePoolImpl* pool);
	~VertexBuffer();

	virtual void Destroy() override {}

private:
	IDirect3DVertexBuffer9* _vb;
};

