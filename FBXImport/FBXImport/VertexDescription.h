#pragma once
#include <d3dx9.h>

//typedef struct Vertex
//{
//	Vector3 Positon;
//	Vector3 Normal;
//	Vector3 UV;
//}VERTEX, *LPVERTEX;

typedef struct tagMeshVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
}MESHVERTEX, *LPMESHVERTEX;

typedef struct tagSkinnedMeshVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;	
	float		Weight[3] = { 0.0f, };			// ����ġ �� 4��
	int			Index[4] = { 0, };			// ���� �ε���
	
}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

struct BOXVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	float x, y, z;	/// ������ ��ȯ�� ��ǥ
	DWORD color;	/// ������ ����
	BOXVERTEX() { x = 0; y = 0; z = 0; color = 0; }
	BOXVERTEX(float f1, float f2, float f3, DWORD c) { x = f1; y = f2; z = f3; color = c; }
};