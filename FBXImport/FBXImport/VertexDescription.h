#pragma once
#include <d3dx9.h>

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
	D3DXVECTOR3	Weight{ 0.0f, 0.0f, 0.0f };	// 가중치 값 4개
	int			Index[4] = { 0, 0, 0, 0};			// 뼈대 인덱스
	
	
}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

typedef struct tagSkinnedMeshVertex2
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR3	Weight{ 0.0f, 0.0f, 0.0f };	// 가중치 값 4개
	int			Index[4] = { 0, 0, 0, 0 };			// 뼈대 인덱스


}SKINNEDMESHVERTEX2, *LPSKINNEDMESHVERTEX2;

struct BOXVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	float x, y, z;	/// 정점의 변환된 좌표
	DWORD color;	/// 정점의 색깔
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


const D3DVERTEXELEMENT9 test[6] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,0 },
	{ 0, 44, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0 },
	D3DDECL_END()
};

/////////////////////////////////////////////////

struct VertexBlendingInfo
{
	unsigned int mBlendingIndex;
	float mBlendingWeight;

	VertexBlendingInfo() :
		mBlendingIndex(0),
		mBlendingWeight(0.0)
	{}

	bool operator < (const VertexBlendingInfo& rhs)
	{
		return (mBlendingWeight > rhs.mBlendingWeight);
	}
};

struct PNTIWVertex
{
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR2 mUV;
	std::vector<VertexBlendingInfo> mVertexBlendingInfos;

	void SortBlendingInfoByWeight()
	{
		std::sort(mVertexBlendingInfos.begin(), mVertexBlendingInfos.end());
	}

	bool operator==(const PNTIWVertex& rhs) const
	{
		bool sameBlendingInfo = true;

		// We only compare the blending info when there is blending info
		if (!(mVertexBlendingInfos.empty() && rhs.mVertexBlendingInfos.empty()))
		{
			// Each vertex should only have 4 index-weight blending info pairs
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mVertexBlendingInfos[i].mBlendingIndex != rhs.mVertexBlendingInfos[i].mBlendingIndex ||
					abs(mVertexBlendingInfos[i].mBlendingWeight - rhs.mVertexBlendingInfos[i].mBlendingWeight) > 0.001)
				{
					sameBlendingInfo = false;
					break;
				}
			}
		}

		bool result1 = (mPosition == rhs.mPosition) ? true : false;
		bool result2 = (mNormal == rhs.mNormal) ? true : false;
		bool result3 = (mUV == rhs.mUV) ? true : false;

		return result1 && result2 && result3 && sameBlendingInfo;
	}
};
