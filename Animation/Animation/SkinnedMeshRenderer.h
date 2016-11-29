#pragma once
#include "MeshRenderer.h"
class SkinnedMeshRenderer :	public MeshRenderer
{
public:
	SkinnedMeshRenderer();
	~SkinnedMeshRenderer();

	virtual void update() override;
};

