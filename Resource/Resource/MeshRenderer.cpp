#include "MeshRenderer.h"
#include "Device.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer()
{
}


MeshRenderer::~MeshRenderer()
{
	_materials.clear();
	_textures.clear();

	SAFE_DELETE(_mesh);
}

void MeshRenderer::update()
{
	DWORD i = 0;
	//DebugBox(_materials.size(), _T("material"));
	for (std::list<D3DMATERIAL9 *>::iterator it = _materials.begin(); it != _materials.end(); ++it)
	{
		D3DDevice->SetMaterial((*it));
		_mesh->_meshContainer->Mesh->DrawSubset(i);		
		i++;
	}
}