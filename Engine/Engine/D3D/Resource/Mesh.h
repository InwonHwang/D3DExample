#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"

class Mesh : public ResourceItem
{
public:
	Mesh(ResourceHandle handle, ResourcePoolImpl* pool);
	~Mesh();

	virtual void Destroy() override;

	bool Create(IDirect3DDevice9& device, sp<FBXMeshData> fbxData);

	sp<VertexBuffer> GetVertexBuffer() const;
	sp<IndexBuffer> GetIndexBuffer() const;
	int	GetVertexCount() const;
	int GetPrimitiveCount() const;
private:
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;

	bool _isSkinned;
	int  _vertexCount;
	int  _primitiveCount;
};

inline sp<VertexBuffer> Mesh::GetVertexBuffer() const
{
	return _vb;
}

inline sp<IndexBuffer> Mesh::GetIndexBuffer() const
{
	return _ib;
}

inline int Mesh::GetVertexCount() const
{
	return _vertexCount;
}

inline int Mesh::GetPrimitiveCount() const
{
	return _primitiveCount;
}

