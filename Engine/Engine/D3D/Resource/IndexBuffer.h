#pragma once
#include "..\..\Core\Core.h"

class IndexBuffer : public ResourceItem
{
public:
	IndexBuffer(ResourceHandle handle, ResourcePoolImpl* pool);
	~IndexBuffer();

	virtual void Destroy() override {}

private:
	IDirect3DIndexBuffer9* _ib;
};

