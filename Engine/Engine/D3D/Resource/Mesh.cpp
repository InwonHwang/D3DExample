#include "Mesh.h"

Mesh::Mesh(ResourceID handle, ResourceTable* pool)
	: ResourceItem(handle, pool),
	_vb(nullptr),
	_ib(nullptr),
	_vertexCount(0),
	_primitiveCount(0)
{
}


Mesh::~Mesh()
{
}

void Mesh::Destroy()
{
	_vb.reset();
	_ib.reset();
	_vertexCount = 0;
	_primitiveCount = 0;
}