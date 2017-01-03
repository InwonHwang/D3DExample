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
	WORD	_0, _1, _2;		/// WORD, 16∫Ò∆Æ ¿Œµ¶Ω∫
#else
	DWORD	_0, _1, _2;		/// WORD, 32∫Ò∆Æ ¿Œµ¶Ω∫
#endif
};
