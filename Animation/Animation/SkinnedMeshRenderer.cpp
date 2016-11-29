#include "SkinnedMeshRenderer.h"
#include "Device.h"
#include "SKinnedMesh.h"

SkinnedMeshRenderer::SkinnedMeshRenderer()
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::update()
{
	DWORD i = 0;
	for (std::list<D3DMATERIAL9 *>::iterator it = _materials.begin(); it != _materials.end(); ++it)
	{
		D3DDevice->SetMaterial((*it));
		_mesh->_meshContainer->Mesh->DrawSubset(i);
		i++;
	}
}
