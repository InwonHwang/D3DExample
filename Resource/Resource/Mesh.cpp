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
void Mesh::loadChildren(LPD3DXFILEDATA, void*)
{

}
void Mesh::load(LPD3DXFILEDATA xfileData)
{
}
