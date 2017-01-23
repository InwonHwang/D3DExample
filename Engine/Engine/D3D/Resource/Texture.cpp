#include "Texture.h"

Texture::Texture(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
}


Texture::~Texture()
{
}

void Texture::LoadTexture(IDirect3DDevice9& device, String fileName)
{
	// 이미 로드 되있으면 리턴
	if (IsLoaded()) return;
	
#ifdef UNICODE 
	if (FAILED(D3DXCreateTextureFromFile(&device, fileName.c_str(), &_texture)))
#elif
	if (FAILED(D3DXCreateTextureFromFileA(&device, fileName.c_str(), &_texture)))
#endif
	{
		//에러 출력
		return;
	}
	
	D3DSURFACE_DESC desc;	
	_texture->GetLevelDesc(0, &desc);
	_width = desc.Width;
	_height = desc.Height;
	SetName(fileName);
	ResourceItem::Load();
}

void Texture::LoadRenderTarget(IDirect3DDevice9& device, String fileName, flag32 state, uint width, uint height)
{	
	// 이미 로드 되있으면 리턴
	if (IsLoaded()) return;

	DWORD usage = D3DUSAGE_RENDERTARGET;

	if (state.check(eMipmap))
	{
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}	


	if (FAILED(device.CreateTexture(width, height, 1, usage,
				D3DFMT_R32F, D3DPOOL_DEFAULT, &_texture, NULL)))
	{
		//에러 출력
		return;
	}
	_state = state;
	_width = width;
	_height = height;
	SetName(fileName);

	ResourceItem::Load();
}

void Texture::Destroy()
{
	//나중에 수정 Memory::Release()
	if (_texture) _texture->Release();

	ResourceItem::Unload();
}
