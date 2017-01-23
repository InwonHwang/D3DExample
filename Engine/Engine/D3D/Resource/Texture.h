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

	void LoadTexture(IDirect3DDevice9& device, String fileName);	// �ؽ��� �ε� �׽�Ʈ ��
	void LoadRenderTarget(IDirect3DDevice9& device, String fileName, flag32 state, uint width, uint height); // ����Ʈ ������ ����� ��.	

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
