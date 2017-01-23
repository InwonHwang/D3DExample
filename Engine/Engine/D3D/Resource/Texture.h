#pragma once
#include "..\..\Core\Core.h"

class Texture : public ResourceItem
{
public:
	enum State
	{
		eMipmap		
	};
public:
	Texture(ResourceHandle handle, ResourcePoolImpl* pool);
	virtual ~Texture();
	
	virtual void Destroy() override;

	uint GetWitdh() const;
	uint GetHeight() const;	
	bool MipmapState() const;
	IDirect3DTexture9* GetD3DTexture() const;

	void LoadTexture(IDirect3DDevice9& device, String fileName);	// 텍스쳐 로드 테스트 용
	void LoadRenderTarget(IDirect3DDevice9& device, String fileName, flag32 state, uint width, uint height); // 이펙트 파일을 사용할 때.	

private:	
	IDirect3DTexture9*		_texture;
	uint					_width;
	uint					_height;	
	flag32					_state;
};

inline uint Texture::GetWitdh() const
{
	return _width;
}

inline uint Texture::GetHeight() const
{
	return _height;
}

inline bool Texture::MipmapState() const
{
	return _state.check(eMipmap);
}

inline IDirect3DTexture9* Texture::GetD3DTexture() const
{
	return _texture;
}
