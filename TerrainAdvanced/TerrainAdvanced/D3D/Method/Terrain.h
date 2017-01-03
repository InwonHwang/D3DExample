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
	IDirect3DVertexBuffer9* _vb; // ������ ������ ��������
	IDirect3DIndexBuffer9*	_ib; // �ε����� ������ �ε�������

	QuadTree*				_quadTree;
	
	Vector3*				_heightMap;			// ���̸��� ������ �迭
	int						_numVertsCol;		// �ؽ�ó�� ����ũ��
	int						_numVertsRow;		// �ؽ�ó�� ����ũ��
	int						_numTriangles;		// �׷��� �ﰢ���� ����
	float					_heightScale;	
	UINT					_cellSpacing;		// �� ����
};