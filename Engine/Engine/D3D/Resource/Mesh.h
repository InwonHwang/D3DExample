#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"
#include "..\D3DDescription\D3DDescription.h"

class Mesh : public ResourceItem
{
public:
	Mesh(ResourceID handle, ResourceTable* pool);
	virtual ~Mesh();

	virtual void Destroy() override;

	virtual bool Create(IDirect3DDevice9& device, sp<FBXMESHDATA> fbxData) = 0;

	sp<VertexBuffer> GetVertexBuffer() const;
	sp<IndexBuffer> GetIndexBuffer() const;
	int	GetVertexCount() const;
	int GetPrimitiveCount() const;

protected:
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;
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

