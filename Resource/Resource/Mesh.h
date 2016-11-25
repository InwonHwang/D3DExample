#pragma once
#include "IResourceItem.h"
#include "Pool.h"

class MeshRenderer;
class SkinnedMeshRenderer;

typedef struct AnimMeshContainer : public D3DXMESHCONTAINER
{
	D3DXMATRIX** _ppFrameMatrices;
	LPD3DXMESH _pFinalMesh;
	LPDIRECT3DTEXTURE9* pTextures;
}ANIMMESHCONTAINER, *LPANIMMESHCONTAINER;

class Mesh : public IResourceItem
{
	friend class MeshRenderer;
protected:
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

class SkinnedMesh : public Mesh
{
	friend class SkinnedMeshRenderer;

public:
	virtual void Save() override;
	virtual void Load(LPD3DXFILEDATA) override;
	virtual void Load(TCHAR *) override;

public:
	SkinnedMesh();
	virtual ~SkinnedMesh();

	void* operator new(size_t size)
	{
		return ResourcePool::Mesh.malloc();
	}

	void operator delete(void* _p)
	{
		ResourcePool::Mesh.free((Mesh * const)_p);
	}
};



