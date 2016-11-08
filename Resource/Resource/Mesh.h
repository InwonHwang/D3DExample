#pragma once
#include "IResourceItem.h"
#include "ResourcePool.h"

class MeshRenderer;

class Mesh : public IResourceItem
{
private:
	LPD3DXMESHCONTAINER _d3dxMeshContainer;

protected:
	virtual void Save() override;
	virtual void Load(LPD3DXFILEDATA) override;
	virtual void Load(TCHAR *) override;
	virtual void draw();


public:
	Mesh();
	~Mesh();
		
	void* operator new(size_t size)
	{
		return ResourcePool::MeshPool.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::MeshPool.free((Mesh * const)_p);
	}
};



