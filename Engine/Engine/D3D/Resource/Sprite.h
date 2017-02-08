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

	// friend class SpriteRenderer private함수로 바꾸기
	IDirect3DVertexBuffer9* GetD3DVertexBuffer() const;
	IDirect3DVertexDeclaration9* GetD3DVertexDeclaration() const;
	IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;
	IDirect3DTexture9*	GetD3DTexture() const;

private:
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;
	sp<Texture>		 _texture;
};

inline IDirect3DVertexBuffer9* Sprite::GetD3DVertexBuffer() const
{
	if(_vb) return _vb->GetD3DVertexBuffer();
	
	return nullptr;
}

inline IDirect3DVertexDeclaration9* Sprite::GetD3DVertexDeclaration() const
{
	if (_vb) return _vb->GetD3DVertexDeclaration();

	return nullptr;
}

inline IDirect3DIndexBuffer9* Sprite::GetD3DIndexBuffer() const
{
	if (_ib) return _ib->GetD3DIndexBuffer();

	return nullptr;
}

inline IDirect3DTexture9*	Sprite::GetD3DTexture() const
{
	if (_texture) return _texture->GetD3DTexture();

	return nullptr;
}
