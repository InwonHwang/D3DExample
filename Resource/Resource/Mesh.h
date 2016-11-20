#pragma once
#include "IResourceItem.h"
#include "Pool.h"

class MeshRenderer;

class Mesh : public IResourceItem
{
	friend class MeshRenderer;
private:
	LPD3DXMESHCONTAINER _d3dxMeshContainer;

//protected:
public:
	virtual void Save() override;
	virtual void Load(LPD3DXFILEDATA) override;
	virtual void Load(TCHAR *) override;

public:
	Mesh();
	virtual ~Mesh();
		
	void* operator new(size_t size)
	{		
		return ResourcePool::Mesh.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::Mesh.free((Mesh * const)_p);
	}
};



