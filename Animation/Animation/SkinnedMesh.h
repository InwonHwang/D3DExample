#pragma once
#include "Mesh.h"

class SkinnedMesh : public Mesh
{
	friend class SkinnedMeshRenderer;
public:
	virtual void save() override;
	virtual void loadChild(LPD3DXFILEDATA, void*) override;
	virtual void load(LPD3DXFILEDATA) override;
	virtual void load(TCHAR *) override;

public:
	SkinnedMesh();
	~SkinnedMesh();
};

