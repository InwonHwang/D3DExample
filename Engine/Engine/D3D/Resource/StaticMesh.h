#pragma once

#include "Mesh.h"

class StaticMesh : public Mesh
{
public:
	StaticMesh(ResourceHandle handle, ResourcePoolImpl* pool);
	~StaticMesh();

	virtual void Destroy() override;

	virtual bool Create(IDirect3DDevice9& device, sp<FBXMESHDATA> fbxData) override;
};

