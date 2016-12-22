#include "ResourceManager.h"
#include <rmxfguid.h>
#include "XFile.h"
#include "Device.h"

#include "Mesh.h"
#include "SkinnedMesh.h"
#include "Animation.h"
#include "AnimationController.h"

#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "Transform.h"

ResourceManager::ResourceManager()
{
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
	SAFE_DELETE(_animationController);
	tempClearTransform(_topTransform);	
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

		parseChild(data, NULL);
		
		SAFE_RELEASE(data);
	}
	
	tempLinkFrame(_topTransform);
	DWORD temp = _animations.front()->getNumBone();
	_animationController = new AnimationController(temp, 1, 1, 1);
	tempRegisterFrame(_animationController, _topTransform);
	DWORD i = 0;
	for (std::list<Animation *>::iterator it = _animations.begin(); it != _animations.end(); ++it)
	{
		_animationController->registerAnimation(i, *it);
	}
	
}

void ResourceManager::parseChild(LPD3DXFILEDATA xfileData, void* data)
{
	HRESULT hr = 0;
	GUID type;

	hr = xfileData->GetType(&type);	
	
	void* param = data;

	if (type == TID_D3DRMFrame) //!skinInfo
	{
		if (!data)
		{
			_topTransform = new Transform();
			_topTransform->_parent = NULL;
			param = _topTransform;
		}
		else
		{
			Transform* parent = static_cast<Transform*>(data);			
			Transform* newTransform = new Transform();
			parent->_children.push_back(newTransform);
			newTransform->_parent = parent;
			param = newTransform;
		}

		DWORD size = 0;

		Transform* tempTransform = static_cast<Transform *>(param);

		if (SUCCEEDED(hr = xfileData->GetName(NULL, &size)))
		{
			if (size > 0) {
				tempTransform->_name = new char[size];
				xfileData->GetName(tempTransform->_name, &size);				
			}
		}
	}
	else if (type == TID_D3DRMFrameTransformMatrix)
	{
		Transform* tempTransform = static_cast<Transform*>(data);		
		tempTransform->_frame->load(xfileData);
		return;
	}
	else if (type == TID_D3DRMMesh) //!skinInfo
	{				
		ID3DXBuffer*	adjBuffer = 0;
		ID3DXBuffer*	mtrlBuffer = 0;
		ID3DXSkinInfo*	skinInfo = 0;
		ID3DXMesh*		mesh = 0;
		DWORD			numMtrls = 0;

		hr = D3DXLoadSkinMeshFromXof(xfileData, D3DXMESH_MANAGED, D3DDevice,
			&adjBuffer, &mtrlBuffer, 0, &numMtrls,&skinInfo, &mesh);

		if (skinInfo)
		{
			_meshRenderer = new SkinnedMeshRenderer();
			_meshRenderer->_mesh = new SkinnedMesh();
		}
		else
		{
			_meshRenderer = new MeshRenderer();
			_meshRenderer->_mesh = new Mesh();			
		}

		_meshRenderer->_mesh->_meshContainer->Mesh = mesh;
		_meshRenderer->_mesh->_meshContainer->Adjacency = adjBuffer;
		_meshRenderer->_mesh->_meshContainer->SkinInfo = skinInfo;

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
			mesh->CloneMesh(D3DXMESH_MANAGED, NULL,	D3DDevice, &_meshRenderer->_mesh->_meshContainer->FinalMesh);		
			_meshRenderer->_mesh->_meshContainer->FrameMatrices = new LPD3DXMATRIX[skinInfo->GetNumBones()];
			_meshRenderer->_mesh->_meshContainer->FinalMatrices = new D3DXMATRIX[skinInfo->GetNumBones()];
		}

		return;
	}
	else if (type == TID_D3DRMAnimationSet)
	{
		DebugBox(0, _T("hello"));
		Animation* animation = new Animation();
		animation->load(xfileData);
		animation->create("temp", 5000, true);
		_animations.push_back(animation);
		return;
	}

	SIZE_T childCount;
	xfileData->GetChildren(&childCount);

	for (SIZE_T i = 0; i < childCount; i++)
	{
		LPD3DXFILEDATA pSubData;
		if (FAILED(hr = xfileData->GetChild(i, &pSubData)))
			continue;

		parseChild(pSubData, param);
		SAFE_RELEASE(pSubData);
	}
}


void ResourceManager::tempUpdateFrame(Transform *transform)
{
	if (!transform) return;
		
	if (transform->_parent)	
		transform->_frame->_matrixContainer->CombTransformationMatrix = transform->_frame->_matrixContainer->Matrix * transform->_parent->_frame->_matrixContainer->CombTransformationMatrix;	
	else
		transform->_frame->_matrixContainer->CombTransformationMatrix = transform->_frame->_matrixContainer->Matrix;

	std::list<Transform *>::iterator it;
	for (it = transform->_children.begin(); it != transform->_children.end(); ++it)	
		tempUpdateFrame(*it);	
}

void ResourceManager::tempUpdate()
{ 
	_animationController->tempUpdate();	
	tempUpdateFrame(_topTransform);		
	_meshRenderer->update();
}

void ResourceManager::tempClearTransform(Transform* transform)
{
	if (!transform) return;	

	SAFE_DELETE(transform);

	std::list<Transform *>::iterator it;	
	for (it = transform->_children.begin(); it != transform->_children.end(); ++it)
	{
		tempClearTransform(*it);
	}
}

void ResourceManager::tempLinkFrame(Transform *transform)
{		
	if (!transform) return;

	ID3DXSkinInfo* skinInfo = _meshRenderer->_mesh->_meshContainer->SkinInfo;
	DWORD numBones = skinInfo->GetNumBones();

	for (DWORD i = 0; i < numBones; i++)
	{
		LPCSTR name = skinInfo->GetBoneName(i);

		if (strcmp(transform->_name, name) == 0)
		{
			_meshRenderer->_mesh->_meshContainer->FrameMatrices[i] = &(transform->_frame->_matrixContainer->CombTransformationMatrix);
		}

	}

	std::list<Transform *>::iterator it;
	for (it = transform->_children.begin(); it != transform->_children.end(); ++it)
	{
		tempLinkFrame(*it);
	}
}

void ResourceManager::tempRegisterFrame(AnimationController* animationController, Transform *transform)
{
	if (!animationController || !transform) return;

	ID3DXSkinInfo* skinInfo = _meshRenderer->_mesh->_meshContainer->SkinInfo;
	DWORD numBones = skinInfo->GetNumBones();

	animationController->registerTransform(transform);

	/*for (DWORD i = 0; i < numBones; i++)
	{
		LPCSTR name = skinInfo->GetBoneName(i);

		if (strcmp(transform->_name, name) == 0)
		{
			MessageBoxA(GetActiveWindow(), name, name, MB_OK);
		}
	}	*/

	std::list<Transform *>::iterator it;
	for (it = transform->_children.begin(); it != transform->_children.end(); ++it)
	{
		tempRegisterFrame(animationController, *it);
	}
}
