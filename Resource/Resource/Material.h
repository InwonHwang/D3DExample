#pragma once
#include "IResourceItem.h"
#include "ResourcePool.h"
#include "stdafx.h"

class Material : public IResourceItem
{
private:
	D3DMATERIAL9	_d3dMaterial;

public:
	Material();
	~Material();

	void* operator new(size_t size)
	{
		return ResourcePool::MaterialPool.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::MaterialPool.free((Material * const)_p);
	}

protected:
	virtual void Save() override {}
	virtual void Load(LPD3DXFILEDATA) override {}
	virtual void Load(TCHAR *) override {}
};

