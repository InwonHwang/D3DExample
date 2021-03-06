#pragma once

#include "..\..\Core\Core.h"

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
	enum {
		FVF = (D3DFVF_XYZ | D3DFVF_TEX3 |
		D3DFVF_TEXCOORDSIZE2(0) |		// texture
			D3DFVF_TEXCOORDSIZE4(1) |		// blend weight
			D3DFVF_TEXCOORDSIZE4(2))
	};

	Vector3 position;
	Vector2 texCoord;
	float	weight[4];		// ����ġ �� 4��
	float	index[4];		// ���� �ε���	

}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

//typedef struct tagSkinnedMeshVertex
//{
//	Vector3 position;
//	Vector4 color;
//	Vector3 normal;
//	Vector2 texCoord;
//	Vector3 tangent;	
//	Vector3	weight;		// ����ġ �� 4��	
//	DWORD	index;	// ���� �ε���
//
//}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

struct TEMPVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
 

	Vector3 position;
	Vector3 normal;
};

struct BOXVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	float x, y, z;	/// ������ ��ȯ�� ��ǥ
	DWORD color;	/// ������ ����
	BOXVERTEX() { x = 0; y = 0; z = 0; color = 0; }
	BOXVERTEX(float f1, float f2, float f3, DWORD c) { x = f1; y = f2; z = f3; color = c; }
};

const D3DVERTEXELEMENT9 StaticMeshDec[6] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	D3DDECL_END()
};

const D3DVERTEXELEMENT9 SkinnedMeshDec[5] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	
	{ 0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	{ 0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },	
	D3DDECL_END()
};

//const D3DVERTEXELEMENT9 SkinnedMeshDec[8] =
//{
//	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
//	{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
//	{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
//	{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
//	{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },		
//	{ 0, 60, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,0 },
//	{ 0, 72, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0 },
//	D3DDECL_END()
//};


