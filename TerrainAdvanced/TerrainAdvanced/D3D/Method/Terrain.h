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
		WORD	_0, _1, _2;		/// WORD, 16��Ʈ �ε���
	};

public:
	Terrain(UINT cellSpacing, float heightScale);
	~Terrain();

	void draw(IDirect3DDevice9* device);
	bool processFrustumCulling(const Frustum& f);
	bool initVertice(IDirect3DTexture9* texHeightMap);
	bool initIndice();

private:
	LPDIRECT3DVERTEXBUFFER9 _vb; // ������ ������ ��������
	LPDIRECT3DINDEXBUFFER9	_ib; // �ε����� ������ �ε�������
	
	Vector3*				_heightMap;	// ���̸��� ������ �迭
	int						_numVertsCol;		// �ؽ�ó�� ����ũ��
	int						_numVertsRow;		// �ؽ�ó�� ����ũ��
	int						_numTriangles;		// �׷��� �ﰢ���� ����
	float					_heightScale;	
	UINT					_cellSpacing;	// �� ����
};