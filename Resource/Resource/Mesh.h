#pragma once
#include "IXFileData.h"
#include "Structure.h"

class Mesh : public IXFileData, public ISubDataParser
{
	friend class MeshRenderer;
	friend class SkinnedMeshRenderer;
	friend class ResourceManager;
protected:
	MESHCONTAINER* _meshContainer;
	//protected:
public:
	virtual void save() override;
	virtual void loadChildren(LPD3DXFILEDATA, void*) override;
	virtual void saveChildren(LPD3DXFILEDATA, void*) override {}
	virtual void load(LPD3DXFILEDATA) override;

public:
	Mesh();
	virtual ~Mesh();
};

