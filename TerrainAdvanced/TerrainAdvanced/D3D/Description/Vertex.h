#pragma once

typedef struct TerrainVertex
{
	Vector3	Position;
	Vector3	Normal;
	Vector3	UV;

	enum FVF { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
}TERRAINVERTEX, *LPTERRAINVERTEX;

struct INDEX
{
#ifdef _USE_INDEX16
	WORD	topLeft, topRight, bottomLeft;		/// WORD, 16∫Ò∆Æ ¿Œµ¶Ω∫
#else
	DWORD	topLeft, topRight, bottomLeft;		/// WORD, 32∫Ò∆Æ ¿Œµ¶Ω∫
#endif
};
