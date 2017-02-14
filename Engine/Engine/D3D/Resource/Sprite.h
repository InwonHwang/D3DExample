#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"

class Sprite : public ResourceItem
{
public:
	Sprite(ResourceHandle handle, ResourcePoolImpl* pool);
	~Sprite();

	virtual void Destroy() override;

	bool Create(IDirect3DDevice9& device, sp<Texture> texture);

	// friend class SpriteRenderer private함수로 바꾸기
	sp<VertexBuffer> GetVertexBuffer() const;
	sp<IndexBuffer> GetIndexBuffer() const;
	sp<Texture>	GetD3DTexture() const;

private:
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;
	sp<Texture>		 _texture;
};

inline sp<VertexBuffer> Sprite::GetVertexBuffer() const
{	
	return _vb;
}


inline sp<IndexBuffer> Sprite::GetIndexBuffer() const
{
	return _ib;
}

inline sp<Texture>	Sprite::GetD3DTexture() const
{
	return _texture;
}
