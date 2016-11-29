#include "Mesh.h"
#include "Device.h"

Mesh::Mesh()
{
	_meshContainer = new MESHCONTAINER();
}


Mesh::~Mesh()
{
	SAFE_DELETE(_meshContainer);
}

void Mesh::save()
{
}
void Mesh::loadChild(LPD3DXFILEDATA, void*)
{

}
void Mesh::load(LPD3DXFILEDATA xfileData)
{
}

void Mesh::load(TCHAR * fileName)
{
	D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, D3DDevice, NULL, NULL, NULL, NULL, &(_meshContainer->Mesh));
}
