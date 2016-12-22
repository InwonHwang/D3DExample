#pragma once
#include "stdafx.h"

typedef struct tagMeshContainer
{
	ID3DXMesh*		Mesh;
	ID3DXMesh*		FinalMesh;
	ID3DXSkinInfo*	SkinInfo;
	ID3DXBuffer*	Adjacency;
	D3DXMATRIX**	FrameMatrices;
	D3DXMATRIX*		FinalMatrices;

//private:
	tagMeshContainer() : Mesh(NULL), SkinInfo(NULL), Adjacency(NULL) {}
	virtual ~tagMeshContainer()
	{	
		SAFE_DELETE_ARRAY(FinalMatrices);
		SAFE_DELETE_ARRAY(FrameMatrices);
		SAFE_RELEASE(Adjacency);
		SAFE_RELEASE(Mesh);
		SAFE_RELEASE(FinalMesh);				
		SAFE_RELEASE(SkinInfo);
		
	}
}MESHCONTAINER, *LPMESHCONTAINER;

typedef struct tagMatrixContainer
{
	D3DXMATRIX CombTransformationMatrix;
	D3DXMATRIX Matrix;
}MATRIXCONTAINER, *LPMATRIXCONTAINER;