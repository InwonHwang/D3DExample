#include "Sprite.h"
#include "..\Description\D3DDescription.h"

extern ResourceManager resourceManager;

Sprite::Sprite(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
}


Sprite::~Sprite()
{
}

void Sprite::Destroy()
{
	_vb.reset();
	_ib.reset();
	_texture.reset();
	ResourceItem::Unload();
	ResourceItem::Clear();
}

bool Sprite::Create(IDirect3DDevice9& device, sp<Texture> texture)
{	
	_vb = resourceManager.Create<VertexBuffer>(); // VertexBuffer Pool로 VertexBuffer만들기	
	_ib = resourceManager.Create<IndexBuffer>(); // IndexBuffer Pool로 IndexBuffer만들기
	
	if (_vb->CreateVertexBuffer(device, sizeof(STATICMESHVERTEX) * 4, 0, D3DPOOL_MANAGED) == false)
		return false;
	
	if (_ib->CreateIndexBuffer(device, sizeof(INDEX) * 2, 0, D3DPOOL_MANAGED) == false)
		return false;	

	if (_vb->CreateVertexDeclaration(device, StaticMeshDec) == false)
		return false;

	// VertexBuffer Lock
	void* pVertexData;
	STATICMESHVERTEX vertex[4];
	float width = texture->GetWitdh() / 2.0f;
	float height = texture->GetHeight() / 2.0f;

	vertex[0].Position = Vector3(-width, height, 0.0f);	// 좌측 위
	vertex[1].Position = Vector3(width, height, 0.0f);		// 우측 위
	vertex[2].Position = Vector3(-width, -height, 0.0f);	// 좌측 아래
	vertex[3].Position = Vector3(width, -height, 0.0f);	// 우측 아래
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		vertex[i].Color = Vector4(255, 255, 255, 255);
		vertex[i].Normal = vertex[i].Position;
		vertex[i].Tangent = Vector3(0, 0, 0);
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

	// IndexBuffer Lock
	void* pIndexData;
	
	if (_ib->Lock(0, sizeof(INDEX) * 2, &pIndexData, 0))
	{
		INDEX* pIndex = static_cast<INDEX*>(pIndexData);

		INDEX index;

		index._0 = 0;
		index._1 = 1;
		index._2 = 2;

		*pIndex++ = index;		
		
		index._0 = 2;
		index._1 = 1;
		index._2 = 3;

		*pIndex = index;	
		
		_ib->Unlock();
	}
	
	ResourceItem::Load();

	return true;
}
