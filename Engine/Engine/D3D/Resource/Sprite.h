#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"

class VertexBuffer;
class IndexBuffer;
class Texture;

class Sprite : public ResourceItem
{
public:
	Sprite(ResourceHandle handle, ResourcePoolImpl* pool);
	~Sprite();

	virtual void Destroy() override;

	bool Create(IDirect3DDevice9& device, sp<Texture> texture);

	//friend class SpriteRenderer private함수로 바꾸기
	IDirect3DVertexBuffer9* GetD3DVertexBuffer() const;
	IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;
	IDirect3DTexture9*	GetD3DTexture() const;

private:
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;
	sp<Texture>		 _texture;
};

IDirect3DVertexBuffer9* Sprite::GetD3DVertexBuffer() const
{
	if(_vb) return _vb->GetD3DVertexBuffer();
	
	return nullptr;
}

IDirect3DIndexBuffer9* Sprite::GetD3DIndexBuffer() const
{
	if (_ib) return _ib->GetD3DIndexBuffer();

	return nullptr;
}

IDirect3DTexture9*	Sprite::GetD3DTexture() const
{
	if (_texture) return _texture->GetD3DTexture();
}
