#pragma once
#include "Mesh.h"
#include "IMemento.h"

class StaticMesh : public Mesh, public IMemento
{
public:
	StaticMesh(IDirect3DDevice9& device);
	virtual ~StaticMesh();

	// inherited from Mesh
	virtual void Draw() override;									// IDirect3DDevice9 �ʿ�

	// inherited from IMemento
	virtual void Load(FbxNode& fbxNode) override;					// IDirect3DDevice9 �ʿ�
	virtual void Save(FbxNode& fbxNode) override;					
	virtual void* GetData() override;

private:
	void InternalLoadVertice(FbxNode& fbxNode);	
	void InternalLoadIndice(FbxNode& fbxNode);
	void InternalSetUpVertexBuffer();
	void InternalSetUpIndexBuffer();
};

