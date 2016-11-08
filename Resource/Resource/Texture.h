#pragma once
#include "IResourceItem.h"
#include "ResourcePool.h"
#include "stdafx.h"

class Texture :	public IResourceItem
{
private:
	LPDIRECT3DTEXTURE9	_d3dTexture;

public:
	Texture();
	~Texture();

	void* operator new(size_t size)
	{
		return ResourcePool::TexturePool.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::TexturePool.free((Texture * const)_p);
	}

protected:
	virtual void Save() override {}
	virtual void Load(LPD3DXFILEDATA) override {}
	virtual void Load(TCHAR *) override {}
};

