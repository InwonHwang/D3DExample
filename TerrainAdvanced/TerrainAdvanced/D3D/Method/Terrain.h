#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "../../Core/Core.h"
#include "../Math/Math.h"

class Frustum;

class Terrain
{
	typedef struct TerrainVertex
	{
		Vector3	Position;
		Vector3	Normal;
		Vector3	UV;

		enum FVF { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	}TERRAINVERTEX, *LPTERRAINVERTEX;

	struct INDEX
	{
		WORD	_0, _1, _2;		/// WORD, 16비트 인덱스
	};

public:
	Terrain(UINT cellSpacing, float heightScale);
	~Terrain();

	void draw(IDirect3DDevice9* device);
	bool processFrustumCulling(const Frustum& f);
	bool initVertice(IDirect3DTexture9* texHeightMap);
	bool initIndice();

private:
	LPDIRECT3DVERTEXBUFFER9 _vb; // 정점을 보관할 정점버퍼
	LPDIRECT3DINDEXBUFFER9	_ib; // 인덱스를 보관할 인덱스버퍼
	
	Vector3*				_heightMap;	// 높이맵을 보관할 배열
	int						_numVertsCol;		// 텍스처의 가로크기
	int						_numVertsRow;		// 텍스처의 세로크기
	int						_numTriangles;		// 그려질 삼각형의 개수
	float					_heightScale;	
	UINT					_cellSpacing;	// 셀 간격
};