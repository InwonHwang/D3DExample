#include "Mesh.h"
#include "Device.h"

Mesh::Mesh()
{
	_d3dxMeshContainer = new D3DXMESHCONTAINER();
}


Mesh::~Mesh()
{	
	SAFE_RELEASE(_d3dxMeshContainer->MeshData.pMesh);
	SAFE_DELETE(_d3dxMeshContainer);
}

void Mesh::Save() 
{
}

void Mesh::Load(LPD3DXFILEDATA xfileData)
{
}

void Mesh::Load(TCHAR * fileName)
{
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, D3DDevice, NULL, NULL, NULL, NULL, &(_d3dxMeshContainer->MeshData.pMesh));
}

