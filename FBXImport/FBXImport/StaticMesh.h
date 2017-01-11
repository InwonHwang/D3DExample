#pragma once
#include "Mesh.h"
#include "IMemento.h"

class StaticMesh : public Mesh, public IMemento
{
public:
	StaticMesh(IDirect3DDevice9& device);
	virtual ~StaticMesh();

	// inherited from IMemento
	virtual void Load(FbxNode& fbxNode, void* pData) override;		// IDirect3DDevice9 �ʿ�
	virtual void Save(FbxNode& fbxNode) override;

	// inherited from Mesh
	virtual void Draw() override;									// IDirect3DDevice9 �ʿ�

protected:
	virtual void LoadVerticeAndIndice(FbxNode& fbxNode, void* pData) override;
};
