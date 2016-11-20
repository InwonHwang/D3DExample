#include "MeshRenderer.h"
#include "Device.h"
#include "Mesh.h"
#include "Material.h"

MeshRenderer::MeshRenderer()
{
}


MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	_mesh = mesh;
}

void MeshRenderer::SetSubMesh(Material* material)
{
	_material = material;
}

void MeshRenderer::Update()
{
	_mesh->_d3dxMeshContainer->MeshData.pMesh->DrawSubset(0);

}


