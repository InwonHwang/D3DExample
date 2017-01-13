#pragma once
#include <d3dx9.h>

//typedef struct Vertex
//{
//	Vector3 Positon;
//	Vector3 Normal;
//	Vector3 UV;
//}VERTEX, *LPVERTEX;

typedef struct tagStaticMeshVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR4 Color;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR3 Tangent;
}STATICMESHVERTEX, *LPSTATICMESHVERTEX;

typedef struct tagSkinnedMeshVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR4 Color;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR3 Tangent;
	float		Weight[3] = { 0.0f, };		// 가중치 값 4개
	int			Index[4] = { 0, };			// 뼈대 인덱스
	
}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

struct BOXVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	float x, y, z;	/// 정점의 변환된 좌표
	DWORD color;	/// 정점의 색깔
	BOXVERTEX() { x = 0; y = 0; z = 0; color = 0; }
	BOXVERTEX(float f1, float f2, float f3, DWORD c) { x = f1; y = f2; z = f3; color = c; }
};

typedef struct tagBlendInfo
{
	DWORD	Index;
	double  Weight;

	tagBlendInfo() :
		Index(0),
		Weight(0)
	{}

	bool operator < (const tagBlendInfo& tDest) { return !(Weight <= tDest.Weight); }
}BLENDINFO, *LPBLENDINFO;

typedef struct tagCtrlPoint {
	STATICMESHVERTEX Vertex;
	std::vector<BLENDINFO> BlendInfo;

	tagCtrlPoint()
	{
		BlendInfo.reserve(4);
	}

	~tagCtrlPoint()
	{
		BlendInfo.erase(BlendInfo.begin(), BlendInfo.end());
		BlendInfo.clear();
	}

	void sort()
	{
		std::sort(BlendInfo.begin(), BlendInfo.end());
	}
}CTRLPOINT, *LPCTRLPOINT;

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