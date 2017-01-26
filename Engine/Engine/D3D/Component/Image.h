#pragma once
#include "..\..\Core\Core.h"
#include "Component.h"

class Texture;

// ���������� UI�� ���. Image�� ���� 4���� ����ϱ�
class Image : public Component
{
public:
	Image();
	~Image();
		
	virtual void Destroy() override;

	void Create(IDirect3DDevice9& device, const sp<Texture>& texture);	
	// �׽�Ʈ ��
	void Draw(IDirect3DDevice9& device);

private:
	ID3DXSprite*	_sprite;
	sp<Texture>		_texture;
};