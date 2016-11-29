#pragma once
#include "IResourceItem.h"
#include "Structure.h"

class Mesh : public IResourceItem
{
	friend class MeshRenderer;
	friend class SkinnedMeshRenderer;
	friend class ResourceManager;
protected:
	MESHCONTAINER* _meshContainer;
	//protected:
public:
	virtual void save() override;
	virtual void loadChild(LPD3DXFILEDATA, void*) override;
	virtual void load(LPD3DXFILEDATA) override;
	virtual void load(TCHAR *) override;	

public:
	Mesh();
	virtual ~Mesh();
};

