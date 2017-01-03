#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "../../Core/Core.h"
#include "../Math/Math.h"
#include "../Description/Description.h"

class Frustum;
class QuadTree;

class Terrain
{


public:
	Terrain(UINT cellSpacing, float heightScale);
	~Terrain();

	virtual void create(IDirect3DDevice9* device, IDirect3DTexture9* texHeightMap);
	virtual void destroy();

	void draw(IDirect3DDevice9* device, Frustum& f);
	bool initVertice(IDirect3DDevice9* device, IDirect3DTexture9* texHeightMap);
	bool initIndice(IDirect3DDevice9* device);

private:
	IDirect3DVertexBuffer9* _vb; // 정점을 보관할 정점버퍼
	IDirect3DIndexBuffer9*	_ib; // 인덱스를 보관할 인덱스버퍼

	QuadTree*				_quadTree;
	
	Vector3*				_heightMap;			// 높이맵을 보관할 배열
	int						_numVertsCol;		// 텍스처의 가로크기
	int						_numVertsRow;		// 텍스처의 세로크기
	int						_numTriangles;		// 그려질 삼각형의 개수
	float					_heightScale;	
	UINT					_cellSpacing;		// 셀 간격
};