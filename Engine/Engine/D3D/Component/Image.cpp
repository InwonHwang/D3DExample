#include "Image.h"
#include "..\Resource\Texture.h"


Image::Image()
{
	Debug::MsgBox(0, _T("持失切"));
}


Image::~Image()
{
	Debug::MsgBox(0, _T("社瑚切"));
}

void Image::Create(IDirect3DDevice9& device, const sp<Texture>& texture)
{
	D3DXCreateSprite(&device, &_sprite);
	_texture = texture;
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
		_sprite->Draw(texture, NULL, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

	_sprite->End();    // end sprite drawing

}