#pragma once
#include "Mesh.h"
#include "IMemento.h"

class StaticMesh : public Mesh, public IMemento
{
public:
	StaticMesh(IDirect3DDevice9& device);
	virtual ~StaticMesh();

	// inherited from Mesh
	virtual void Draw() override;									// IDirect3DDevice9 필요

	// inherited from IMemento
	virtual void Load(FbxNode& fbxNode, void* pData) override;		// IDirect3DDevice9 필요
	virtual void Save(FbxNode& fbxNode) override;

protected:
	// inherited from Mesh
	virtual void LoadPosition(FbxMesh& mesh, int ctrlPointIndex, D3DXVECTOR3& position) override;
	virtual void LoadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR4& color) override;
	virtual void LoadUV(FbxMesh& mesh, int ctrlPointIndex, int uvIndex, D3DXVECTOR2& uv) override;
	virtual void LoadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& normal) override;
	virtual void LoadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& tangent) override;
};
