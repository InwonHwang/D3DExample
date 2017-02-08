#include "SpriteRenderer.h"
#include "..\Description\D3DDescription.h"


SpriteRenderer::SpriteRenderer()
{
	_materials.reset(new MaterialVec);	
}


SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Destroy()
{
	_materials->clear();
	_materials.reset();
	_sprite.reset();
}

void SpriteRenderer::Draw(IDirect3DDevice9& device)
{
	if (!_sprite) return;

	device.SetStreamSource(0, _sprite->GetD3DVertexBuffer(), 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(_sprite->GetD3DVertexDeclaration());
	device.SetIndices(_sprite->GetD3DIndexBuffer());	
	
	uint materialCount = _materials->size();

	for (uint i = 0; i < materialCount; ++i)
	{
		sp<SurfaceMaterial> material = _materials->data()[i];

		uint numPass;
		material->Begin(numPass);

		for (uint j = 0; j < numPass; ++j)
		{
			material->BeginPass(j);

			device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

			material->EndPass();
		}		

		material->End();
	}
}

void SpriteRenderer::SetSprite(const sp<Sprite> sprite)
{
	_sprite = sprite;	
}

void SpriteRenderer::SetMaterialCount(uint count)
{
	_materials->resize(count);
}

void SpriteRenderer::SetMaterial(uint index, const sp<SurfaceMaterial> material)
{
	assert(index < _materials->size());
	_materials->data()[index] = material;
}
