#pragma once
#include "..\..\Core\Core.h"

class Texture;

// 실제적으로 UI로 사용. Image는 정점 4개로 출력하기
class Image : public ResourceItem
{
public:
	Image(ResourceHandle handle, ResourcePoolImpl* pool);
	~Image();

	void Create(IDirect3DDevice9& device, const sp<Texture>& texture, RECT rect);
	void Destroy();
	// 테스트 용
	void Draw(IDirect3DDevice9& device);
	

private:
	ID3DXSprite*	_sprite;
	wp<Texture>		_texture;	
	RECT _rect;
};