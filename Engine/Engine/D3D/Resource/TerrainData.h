#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"

class TerrainData : public ResourceItem
{
public:
	TerrainData(ResourceHandle handle, ResourcePoolImpl* pool);
	~TerrainData();

	virtual void Destroy() override;

	bool Create(IDirect3DDevice9& device, int width, const std::vector<BYTE>& height);
	bool Create(IDirect3DDevice9& device, const String& fileName);

	uint GetVerticeCount() const;
	uint GetPrimitiveCount() const;
	uint GetWidth() const;
	Vector3 GetHeightData(uint x, uint z) const;

	IDirect3DVertexBuffer9* GetD3DVertexBuffer() const;
	IDirect3DVertexDeclaration9* GetD3DVertexDeclaration() const;
	IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;
	
private:
	sp<std::vector<Vector3>>		 _heightMap;
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;
	uint			 _verticeCount;
	uint			 _primitiveCount;
	uint			 _width;
};


inline uint TerrainData::GetVerticeCount() const
{
	return _verticeCount;
}

inline uint TerrainData::GetPrimitiveCount() const
{
	return _primitiveCount;
}

inline uint TerrainData::GetWidth() const
{
	return _width;
}

inline Vector3 TerrainData::GetHeightData(uint x, uint z) const
{
	return _heightMap->data()[x + _width * z];
}

inline IDirect3DVertexBuffer9* TerrainData::GetD3DVertexBuffer() const
{
	assert(_vb && "null reference: _vb");
	return _vb->GetD3DVertexBuffer();
}

inline IDirect3DVertexDeclaration9* TerrainData::GetD3DVertexDeclaration() const
{
	assert(_vb && "null reference: _vb");
	return _vb->GetD3DVertexDeclaration();
}

inline IDirect3DIndexBuffer9* TerrainData::GetD3DIndexBuffer() const
{
	assert(_ib && "null reference: _ib");
	return _ib->GetD3DIndexBuffer();
}

