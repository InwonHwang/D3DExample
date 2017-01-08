#pragma once
#include <d3dx9.h>

//typedef struct Vertex
//{
//	Vector3 Positon;
//	Vector3 Normal;
//	Vector3 UV;
//}VERTEX, *LPVERTEX;

typedef struct MeshVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
}MESHVERTEX, *LPMESHVERTEX;