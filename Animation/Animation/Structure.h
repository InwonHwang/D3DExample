#pragma once
#include "stdafx.h"

typedef struct tagMeshContainer
{
	ID3DXMesh*		Mesh;
	ID3DXMesh*		FinalMesh;
	ID3DXSkinInfo*	SkinInfo;
	DWORD*			Adjacency;
	D3DXMATRIX**	FrameMatrices;

//private:
	tagMeshContainer() : Mesh(NULL), SkinInfo(NULL), Adjacency(NULL) {}
	virtual ~tagMeshContainer()
	{
		SAFE_DELETE_ARRAY(FrameMatrices);
		SAFE_DELETE_ARRAY(Adjacency);
		SAFE_RELEASE(SkinInfo);
		SAFE_RELEASE(Mesh);		
		SAFE_RELEASE(FinalMesh);
	}
}MESHCONTAINER, *LPMESHCONTAINER;

typedef struct tagMatrixContainer
{
	D3DMATRIX CombTransformationMatrix;
	D3DMATRIX Matrix;
}MATRIXCONTAINER, *LPMATRIXCONTAINER;