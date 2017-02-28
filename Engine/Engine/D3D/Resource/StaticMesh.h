#pragma once

#include "Mesh.h"

class StaticMesh : public Mesh
{
public:
	StaticMesh(ResourceID handle, ResourceTable* pool);
	~StaticMesh();

	virtual void Destroy() override;

	virtual bool Create(IDirect3DDevice9& device, sp<FBXMESHDATA> fbxData) override;
};

