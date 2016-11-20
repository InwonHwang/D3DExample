#pragma once
#include "IResourceItem.h"
#include "Pool.h"
#include "stdafx.h"

class Texture :	public IResourceItem
{
private:
	LPDIRECT3DTEXTURE9	_d3dTexture;

//protected:
public:
	virtual void Save() override {}
	virtual void Load(LPD3DXFILEDATA) override {}
	virtual void Load(TCHAR *) override {}

public:
	Texture();
	~Texture();

	void* operator new(size_t size)
	{
		return ResourcePool::Texture.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::Texture.free((Texture * const)_p);
	}
};

