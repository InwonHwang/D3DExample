#include "Image.h"



Image::Image(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
}


Image::~Image()
{
}

void Image::Create(IDirect3DDevice9& device, const sp<Texture>& texture, RECT rect)
{
	D3DXCreateSprite(&device, &_sprite);
	_texture = texture;
	_rect = rect;
}

void Image::Destroy()
{
	if (_sprite) _sprite->Release();
}

void Image::Draw(IDirect3DDevice9& device)
{
	if (_texture.expired()) return;

	_sprite->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

	IDirect3DTexture9* texture = _texture.lock()->GetD3DTexture();
						 
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);   
	D3DXVECTOR3 position(50.0f, 50.0f, 0.0f); 

	if(texture)
		_sprite->Draw(texture, NULL, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));

	_sprite->End();    // end sprite drawing

}