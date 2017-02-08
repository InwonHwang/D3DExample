#pragma once
#include "..\..\Core\Core.h"
#include "Component.h"
#include "..\Resource\D3DResource.h"


class SpriteRenderer : public Component
{
	typedef std::vector<sp<SurfaceMaterial>> MaterialVec;
public:
	SpriteRenderer();
	~SpriteRenderer();

	virtual void Destroy();	
	virtual void Draw(IDirect3DDevice9& device);	// Renderer���� ��� ����

	void SetSprite(const sp<Sprite> sprite);
	void SetMaterialCount(uint count);
	void SetMaterial(uint index, const sp<SurfaceMaterial> material);
	

private:
	sp<MaterialVec>		_materials;
	sp<Sprite>			_sprite;
	
};

