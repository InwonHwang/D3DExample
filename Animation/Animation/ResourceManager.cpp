#include "ResourceManager.h"
#include <rmxfguid.h>
#include "XFile.h"
#include "Device.h"

#include "Mesh.h"
#include "SkinnedMesh.h"
#include "Animation.h"

#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"

ResourceManager::ResourceManager()
{
	i = 0;
}


ResourceManager::~ResourceManager()
{
	for (std::list<Animation *>::iterator it = _animations.begin(); it != _animations.end(); ++it)
	{		
		SAFE_DELETE((*it));
	}
	_animations.clear();

	for (std::list<IDirect3DTexture9 *>::iterator it = _textures.begin(); it != _textures.end(); ++it)
	{		
		SAFE_RELEASE((*it));
	}
	_textures.clear();

	SAFE_DELETE(_meshRenderer);
}

void ResourceManager::parse(char* fileName)
{
	LPD3DXFILEENUMOBJECT pEnumObject = NULL;
	HRESULT hr = 0;

	if (FAILED(hr = D3DXFILE->CreateEnumObject(fileName, DXFILELOAD_FROMFILE, &pEnumObject)))
	{
		DebugError(hr);
	}

	SIZE_T childCount = 0;
	pEnumObject->GetChildren(&childCount);

	for (SIZE_T i = 0; i < childCount; i++)
	{
		LPD3DXFILEDATA data = NULL;

		if (FAILED(hr = pEnumObject->GetChild(i, &data)))
		{
			DebugError(hr);
			break;
		}

		parseChild(data);
		
		SAFE_RELEASE(data);
	}
}

void ResourceManager::parseChild(LPD3DXFILEDATA xfileData)
{
	HRESULT hr = 0;
	GUID type;

	hr = xfileData->GetType(&type);	

	if (type == TID_D3DRMMesh && i == 0) //!skinInfo)
	{		
		ID3DXBuffer*	adjBuffer = 0;
		ID3DXBuffer*	mtrlBuffer = 0;
		ID3DXSkinInfo*	skinInfo = 0;
		ID3DXMesh*		mesh = 0;
		DWORD			numMtrls = 0;

		hr = D3DXLoadSkinMeshFromXof(xfileData, D3DXMESH_MANAGED, D3DDevice,
			&adjBuffer, &mtrlBuffer, 0, &numMtrls,&skinInfo, &mesh);

		Mesh* tempMesh = NULL;
		if (skinInfo)
		{
			tempMesh = new SkinnedMesh();
			_meshRenderer = new SkinnedMeshRenderer();
		}
		else
		{
			tempMesh = new Mesh();
			_meshRenderer = new MeshRenderer();
		}
			

		tempMesh->_meshContainer->Mesh = mesh;
		tempMesh->_meshContainer->Adjacency = (DWORD*)adjBuffer->GetBufferPointer();
		tempMesh->_meshContainer->SkinInfo = skinInfo;
		_meshRenderer->_mesh = tempMesh;

		if (mtrlBuffer != 0 && numMtrls != 0)
		{
			D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

			for (DWORD i = 0; i < numMtrls; i++)
			{
				mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

				_materials.push_back(&mtrls[i].MatD3D);
				_meshRenderer->_materials.push_back(&mtrls[i].MatD3D);
				if (mtrls[i].pTextureFilename != 0)
				{
					IDirect3DTexture9* tex = 0;
					D3DXCreateTextureFromFile(D3DDevice, (TCHAR *)mtrls[i].pTextureFilename, &tex);
					_textures.push_back(tex);
					_meshRenderer->_textures.push_back(tex);
				}
				else
				{
					_textures.push_back(0);
					_meshRenderer->_textures.push_back(0);
				}

			}
		}

		SAFE_RELEASE(adjBuffer);
		SAFE_RELEASE(mtrlBuffer);

		if (skinInfo)
		{
			mesh->CloneMesh(D3DXMESH_MANAGED, NULL,	D3DDevice, &tempMesh->_meshContainer->FinalMesh);
		}
		i++;
	}
	if (type == TID_D3DRMAnimationSet)
	{
		/*Animation* animation = new Animation();
		animation->load(xfileData);
		_animations.push_back(animation);*/
	}


	SIZE_T childCount;
	xfileData->GetChildren(&childCount);

	for (SIZE_T i = 0; i < childCount; i++)
	{
		LPD3DXFILEDATA pSubData;
		if (FAILED(hr = xfileData->GetChild(i, &pSubData)))
			continue;

		parseChild(pSubData);
		SAFE_RELEASE(pSubData);
	}
}

void ResourceManager::tempUpdate() { _meshRenderer->update(); }