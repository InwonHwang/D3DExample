#include "Mesh.h"
#include "Device.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{	
}

void Mesh::Save() 
{
}

void Mesh::Load(LPD3DXFILEDATA xfileData)
{
}

void Mesh::Load(TCHAR * fileName)
{
	if (!_d3dxMeshContainer) return; // 일단 리턴하는 방식 추후에 MeshContainer가 null이면 생성 해 주는 코드로 변겅

	LPD3DXBUFFER temp;
	LPD3DXMESH tempMesh;
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, D3DDevice, &temp, NULL, NULL, NULL, &tempMesh);
}

void Mesh::draw(DWORD i)
{
	_d3dxMeshContainer->MeshData.pMesh->DrawSubset(i);
}

