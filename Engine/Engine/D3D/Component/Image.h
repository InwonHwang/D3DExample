#pragma once
#include "..\..\Core\Core.h"
#include "Component.h"

class Texture;

// 실제적으로 UI로 사용. Image는 정점 4개로 출력하기
class Image : public Component
{
public:
	Image();
	~Image();
		
	virtual void Destroy() override;

	void Create(IDirect3DDevice9& device, const sp<Texture>& texture);	
	// 테스트 용
	void Draw(IDirect3DDevice9& device);

private:
	ID3DXSprite*	_sprite;
	sp<Texture>		_texture;
};