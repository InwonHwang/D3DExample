#pragma once
#include "IResourceItem.h"
#include "Pool.h"
#include "stdafx.h"

class Material : public IResourceItem
{
private:
	D3DMATERIAL9	_d3dMaterial;

//protected:
public:
	virtual void Save() override {}
	virtual void Load(LPD3DXFILEDATA) override {}
	virtual void Load(TCHAR *) override {}
	void set()
	{
		_d3dMaterial.Ambient = D3DXCOLOR(255, 255, 0, 255);
		_d3dMaterial.Diffuse = D3DXCOLOR(255, 255, 0, 255);
		_d3dMaterial.Specular = D3DXCOLOR(255, 255, 0, 255);
		_d3dMaterial.Emissive = D3DXCOLOR(0, 0, 0, 255);
		_d3dMaterial.Power = 2.0f;
	}

public:
	Material();
	~Material();

	void* operator new(size_t size)
	{		
		return ResourcePool::Material.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::Material.free((Material * const)_p);
	}
};