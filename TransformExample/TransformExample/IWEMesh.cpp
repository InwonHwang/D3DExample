#include "IWEMesh.h"
#include "IWETransform.h"
#include "IWEGameObject.h"
#include "Device.h"

Mesh::Mesh()
	: pMeshData(NULL),
	materials(NULL),
	texture(NULL),
	numMaterials(0)
{
	pMeshData = new D3DXMESHDATA();
}


Mesh::~Mesh()
{	
	
	RELEASE_COM(pMeshData->pMesh);
	RELEASE_COM(pMeshData->pPatchMesh);
	RELEASE_COM(pMeshData->pPMesh);
	SAFE_DELETE(pMeshData);
	SAFE_DELETE_ARRAY(materials);
}


void Mesh::update()
{
	for (DWORD i = 0; i < numMaterials; i++)
	{
		g_Device->SetMaterial(&materials[i]);
		if (texture[i] != NULL)   
			g_Device->SetTexture(0, texture[i]);

		pMeshData->pMesh->DrawSubset(i);
	}
}