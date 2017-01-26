#include "Texture.h"

Texture::Texture(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
}


Texture::~Texture()
{
}

void Texture::Destroy()
{
	SafeRelease<IDirect3DTexture9>(_texture);
	ResourceItem::Unload();
	ResourceItem::Clear();
}

void Texture::CreateRenderTarget(IDirect3DDevice9& device, String fileName, flag32 state, uint width, uint height)
{
	// �̹� �ε� �������� ����
	if (IsLoaded()) return;

	DWORD usage = D3DUSAGE_RENDERTARGET;
	HRESULT hr = 0;

	if (state.check(eMipmap))
	{
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}

	
	if (FAILED(hr = device.CreateTexture(width, height, 1, usage,
		D3DFMT_R32F, D3DPOOL_DEFAULT, &_texture, NULL)))
	{
		DebugError(hr);
		return;
	}

	if (IsBackup()) // ��� �ɼ��� ���� ������ ���� ��� ������ ����
	{		
	}

	_state = state;
	_width = width;
	_height = height;
	SetName(fileName);

	ResourceItem::Load();
}


void Texture::CreateTexture(IDirect3DDevice9& device, String fileName)
{
	// �̹� �ε� �������� ����
	if (IsLoaded()) return;
	
	HRESULT hr = 0;
#ifdef UNICODE 
	if (FAILED(hr = D3DXCreateTextureFromFile(&device, fileName.c_str(), &_texture)))
#elif
	if (FAILED(hr = D3DXCreateTextureFromFileA(&device, fileName.c_str(), &_texture)))
#endif
	{
		DebugError(hr);
		return;
	}

	if (IsBackup()) // ��� �ɼ��� ���� ������ ���� ��� ������ ����
	{
	}
	
	D3DSURFACE_DESC desc;	
	_texture->GetLevelDesc(0, &desc);
	_width = desc.Width;
	_height = desc.Height;
	SetName(fileName);
	ResourceItem::Load();
}



