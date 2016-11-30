#include "SkinnedMeshRenderer.h"
#include "Device.h"
#include "SKinnedMesh.h"

SkinnedMeshRenderer::SkinnedMeshRenderer()
	: MeshRenderer()
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{	
}

void SkinnedMeshRenderer::update()
{
	updateMeshContainer();

	DWORD i = 0;
	std::list<D3DMATERIAL9 *>::iterator it = _materials.begin();

	for (DWORD i = 0; i < _materials.size(); i++)
	{
		D3DDevice->SetMaterial((*it));
		_mesh->_meshContainer->Mesh->DrawSubset(i);
		++it;
	}
}

void SkinnedMeshRenderer::updateMeshContainer()
{
	if(_mesh->_meshContainer && _mesh->_meshContainer->SkinInfo)
	{
		UINT NumFrames = _mesh->_meshContainer->SkinInfo->GetNumBones();    // find how many frames
																	  // for each frame in the mesh container...
		for (UINT i = 0; i < NumFrames; i++)
		{
			_mesh->_meshContainer->FinalMatrices[i] = *_mesh->_meshContainer->SkinInfo->GetBoneOffsetMatrix(i);			
			_mesh->_meshContainer->FinalMatrices[i] *= *_mesh->_meshContainer->FrameMatrices[i];			
		}

		void* pSrc = NULL;   
		void* pDst = NULL;    

							 
		_mesh->_meshContainer->Mesh->LockVertexBuffer(NULL, &pSrc);
		_mesh->_meshContainer->FinalMesh->LockVertexBuffer(NULL, &pDst);

		// store the animated mesh into FinalMesh
		_mesh->_meshContainer->SkinInfo->UpdateSkinnedMesh(_mesh->_meshContainer->FinalMatrices, NULL, pSrc, pDst);

		// unlock the two meshes
		_mesh->_meshContainer->FinalMesh->UnlockVertexBuffer();
		_mesh->_meshContainer->Mesh->UnlockVertexBuffer();
	}
}
