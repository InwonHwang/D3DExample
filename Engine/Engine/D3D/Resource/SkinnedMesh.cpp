#include "SkinnedMesh.h"

extern ResourceManager resourceManager;

SkinnedMesh::SkinnedMesh(ResourceID handle, ResourceTable* pool)
	: Mesh(handle, pool)
{
	_boneIndexVec.reset(new BoneIndexVec);
}


SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::Destroy()
{
	Mesh::Destroy();
	_boneIndexVec.reset();
	SafeDeleteArray<SKINNEDMESHVERTEX>(pSkinnedMeshVertex);
	SafeDeleteArray<TEMPVERTEX>(pVertexDraw);
}

bool SkinnedMesh::Create(IDirect3DDevice9& device, sp<FBXMESHDATA> fbxData)
{
	assert(fbxData && "null reference: fbxData");

	_vertexCount = fbxData->verticeDataVec.size();
	_primitiveCount = fbxData->indiceDataVec.size();

	pSkinnedMeshVertex = new SKINNEDMESHVERTEX[_vertexCount];
	pVertexDraw = new TEMPVERTEX[_vertexCount];
	pIndex = (void*)(new INDEX[_primitiveCount]);

	assert(_vertexCount && " does not exist vertice data");
	assert(_primitiveCount && " does not exist indice data");

	_vb = resourceManager.Create<VertexBuffer>();
	_ib = resourceManager.Create<IndexBuffer>();

	if (!_vb->CreateVertexBuffer(device, _vertexCount * sizeof(SKINNEDMESHVERTEX), 0, SKINNEDMESHVERTEX::FVF, D3DPOOL_MANAGED))
		return false;

	if (!_ib->CreateIndexBuffer(device, _primitiveCount * sizeof(INDEX), 0, D3DPOOL_MANAGED))
		return false;

	//if (!_vb->CreateVertexDeclaration(device, SkinnedMeshDec))
	//	return false;

	//std::set<int> boneIndex;	// 정점에 연결된 본 인덱스

	void * pData = nullptr;
	if (_vb->Lock(0, _vertexCount * sizeof(SKINNEDMESHVERTEX), &pData, 0))
	{
		SKINNEDMESHVERTEX* pVertice = static_cast<SKINNEDMESHVERTEX*>(pData);
		SKINNEDMESHVERTEX vertex;
		//DWORD idx[4];

		for (int i = 0; i < _vertexCount; ++i)
		{
			vertex.position = fbxData->verticeDataVec[i].vertex.position;
			//vertex.color = fbxData->verticeDataVec[i].vertex.color;
			//vertex.normal = fbxData->verticeDataVec[i].vertex.normal;
			vertex.texCoord = fbxData->verticeDataVec[i].vertex.texCoord;
			//vertex.tangent = fbxData->verticeDataVec[i].vertex.tangent;


			
			for (int j = 0; j < 4; ++j)
			{				
				//idx[j] = fbxData->verticeDataVec[i].vertexBlendingInfoVec[j].blendingIndex;
				vertex.index[j] = float(fbxData->verticeDataVec[i].vertexBlendingInfoVec[j].blendingIndex);
				//boneIndex.insert(idx[j]);
				vertex.weight[j] = float(fbxData->verticeDataVec[i].vertexBlendingInfoVec[j].blendingWeight);
			}

			//vertex.index = ((idx[3]) << 24 | (idx[2]) << 16 | (idx[1]) << 8 | (idx[0]));
			
			pSkinnedMeshVertex[i] = vertex;
			*pVertice++ = vertex;
		}

		_vb->Unlock();
	}

	//_boneIndexVec->assign(boneIndex.begin(), boneIndex.end());
	
	pData = nullptr;
	if (_ib->Lock(0, _primitiveCount * sizeof(INDEX), &pData, 0))
	{
		INDEX* pIndice = static_cast<INDEX*>(pData);
		INDEX index;

		//
		DWORD* pW = (DWORD*)pIndex;

		for (int i = 0; i < _primitiveCount;)
		{
			for (int j = 0; j < 3; j++)
			{
				index._0 = fbxData->indiceDataVec[i++];
				index._1 = fbxData->indiceDataVec[i++];
				index._2 = fbxData->indiceDataVec[i++];
				*(pW + 0) = index._0;
				*(pW + 1) = index._1;
				*(pW + 2) = index._2;
				pW += 3;
				*pIndice++ = index;
			}
		}

		_ib->Unlock();
	}

	return true;
}