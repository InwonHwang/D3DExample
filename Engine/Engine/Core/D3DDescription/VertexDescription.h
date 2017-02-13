#pragma once

#include "..\Math\Math.h"
#include "..\DataStructure.h"

typedef struct tagStaticMeshVertex
{
	Vector3 position;
	Vector4 color;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;
}STATICMESHVERTEX, *LPSTATICMESHVERTEX;


typedef struct tagSkinnedMeshVertex
{
	Vector3 position;
	Vector4 color;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;
	Vector3	weight{ 0.0f, 0.0f, 0.0f };	// ����ġ �� 4��
	int		index[4] = { 0, 0, 0, 0 };	// ���� �ε���


}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

typedef struct tagSkinInfo
{
	std::vector<int>	id;
	std::vector<float>	weightVec;
	~tagSkinInfo()
	{
		id.swap(std::vector<int>());
		weightVec.swap(std::vector<float>());
	}
}SKININFO, *LPSKININFO;


const D3DVERTEXELEMENT9 StaticMeshDec[6] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	D3DDECL_END()
};

const D3DVERTEXELEMENT9 SkinnedMeshDec[8] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	{ 0, 60, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,0 },
	{ 0, 72, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0 },
	D3DDECL_END()
};


