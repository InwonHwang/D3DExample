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
	Texture(ResourceID handle, ResourceTable* pool);
	virtual ~Texture();
	
	virtual void Destroy() override;

	bool CreateTexture(IDirect3DDevice9& device, const String& fileName);	// �ؽ��� �ε� �׽�Ʈ ��
	bool CreateRenderTarget(IDirect3DDevice9& device, const String& fileName, flag32 state, uint width, uint height); // ����Ʈ ������ ����� ��.	

	uint GetWitdh() const;
	uint GetHeight() const;	
	bool Mipmap() const;
	IDirect3DTexture9* GetD3DTexture() const;

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

inline bool Texture::Mipmap() const
{
	return _state.check(eMipmap);
}

inline IDirect3DTexture9* Texture::GetD3DTexture() const
{
	return _texture;
}
