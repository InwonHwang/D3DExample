#pragma once
#include "..\..\Core\Core.h"

class Sprite : public ResourceItem
{
public:
	Sprite(ResourceHandle handle, ResourcePoolImpl* pool);
	~Sprite();

	virtual void Destroy() override {}
};

