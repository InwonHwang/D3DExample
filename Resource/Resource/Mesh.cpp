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
	if (!_d3dxMeshContainer) return; // �ϴ� �����ϴ� ��� ���Ŀ� MeshContainer�� null�̸� ���� �� �ִ� �ڵ�� ����

	LPD3DXBUFFER temp;
	LPD3DXMESH tempMesh;
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, D3DDevice, &temp, NULL, NULL, NULL, &tempMesh);
}

void Mesh::draw(DWORD i)
{
	_d3dxMeshContainer->MeshData.pMesh->DrawSubset(i);
}

