#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"
#include "..\D3DDescription\D3DDescription.h"

class TerrainData : public ResourceItem
{
	typedef std::vector<Vector3> HeightMapVec;
public:
	TerrainData(ResourceID handle, ResourceTable* pool);
	~TerrainData();

	virtual void Destroy() override;

	bool Create(IDirect3DDevice9& device, int width, const std::vector<BYTE>& height);
	bool Create(IDirect3DDevice9& device, const String& fileName);

	// friend class SpriteRenderer private함수로 바꾸기
	int GetVerticeCount() const;
	int GetWidth() const;
	sp<HeightMapVec> GetHeightMap() const;

	sp<VertexBuffer> GetVertexBuffer() const;
	sp<IndexBuffer> GetIndexBuffer() const;
	
private:
	sp<HeightMapVec> _heightMap;
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;
	int				 _verticeCount;
	int				 _width;
};


inline int TerrainData::GetVerticeCount() const
{
	return _verticeCount;
}

inline int TerrainData::GetWidth() const
{
	return _width;
}

inline sp<TerrainData::HeightMapVec> TerrainData::GetHeightMap() const
{
	assert(_heightMap && "null reference: _heightMap");
	return _heightMap;
}

inline sp<VertexBuffer> TerrainData::GetVertexBuffer() const
{
	return _vb;
}

inline sp<IndexBuffer> TerrainData::GetIndexBuffer() const
{
	return _ib;
}

