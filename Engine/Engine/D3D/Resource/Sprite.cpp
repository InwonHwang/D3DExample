#include "Sprite.h"
#include "..\Description\D3DDescription.h"


Sprite::Sprite(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
}


Sprite::~Sprite()
{
}

void Sprite::Destroy()
{
	_vb = nullptr;
	_ib = nullptr;
	_texture = nullptr;
}

bool Sprite::Create(IDirect3DDevice9& device, sp<Texture> texture)
{
	// VertexBuffer Pool로 VertexBuffer만들기
	if (!_vb->GetD3DVertexBuffer() && _vb->CreateVertexBuffer(device, sizeof(STATICMESHVERTEX) * 4, 0, D3DPOOL_MANAGED) == false)
		return false;
	// IndexBuffer Pool로 IndexBuffer만들기
	if (!_ib->GetD3DIndexBuffer() && _ib->Create(device, sizeof(unsigned long) * 6, 0, D3DPOOL_MANAGED) == false)
		return false;

	// VertexBuffer Lock
	void* pVertexData;
	STATICMESHVERTEX vertex[4];
	float width = texture->GetWitdh();
	float height = texture->GetWitdh();

	vertex[0].Position = Vector3(-width / 2, height / 2, 0);	// 좌측 위
	vertex[1].Position = Vector3(width / 2, height / 2, 0);		// 우측 위
	vertex[2].Position = Vector3(-width / 2, -height / 2, 0);	// 좌측 아래
	vertex[3].Position = Vector3(width / 2, -height / 2, 0);	// 우측 아래
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		vertex[i].Color = Vector4(255, 255, 255, 255);
		vertex[i].Normal = vertex[i].Position;
		
	}

	if (_vb->Lock(0, sizeof(STATICMESHVERTEX) * 4, &pVertexData, 0))
	{
		STATICMESHVERTEX* pVertex = static_cast<STATICMESHVERTEX*>(pVertexData);
		
		for (int i = 0; i < 4; ++i)
		{
			*pVertex++ = vertex[i];
		}

		_vb->Unlock();
	}

	void* pIndexData;

	if (_ib->Lock(0, sizeof(unsigned long) * 6, &pIndexData, 0))
	{
		unsigned long* pIndex = static_cast<unsigned long*>(pIndexData);

		*pIndex++ = 0;
		*pIndex++ = 1;
		*pIndex++ = 2;
		
		*pIndex++ = 2;
		*pIndex++ = 1;
		*pIndex++ = 3;		

		_ib->Unlock();
	}
	

	// Lock Unlock하기

	return true;
}
