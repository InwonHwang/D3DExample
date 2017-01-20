#pragma once
#include "..\..\Core\Core.h"

class Texture;

// ���������� UI�� ���. Image�� ���� 4���� ����ϱ�
class Image : public ResourceItem
{
public:
	Image(ResourceHandle handle, ResourcePoolImpl* pool);
	~Image();

	void Create(IDirect3DDevice9& device, const sp<Texture>& texture, RECT rect);
	void Destroy();
	// �׽�Ʈ ��
	void Draw(IDirect3DDevice9& device);
	

private:
	ID3DXSprite*	_sprite;
	wp<Texture>		_texture;	
	RECT _rect;
};