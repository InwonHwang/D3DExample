#pragma once
#include "stdafx.h"

class Transform;

class Mesh
{

public:
	DWORD				 numMaterials;

	LPD3DXMESHDATA		 pMeshData;
	D3DMATERIAL9*		 materials;
	LPDIRECT3DTEXTURE9*  texture;
	
	void update();
	
public:
	Mesh();
	~Mesh();

};

