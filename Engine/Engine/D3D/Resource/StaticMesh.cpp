#include "StaticMesh.h"

extern ResourceManager resourceManager;

StaticMesh::StaticMesh(ResourceID handle, ResourceTable* pool)
	: Mesh(handle, pool)
{
}


StaticMesh::~StaticMesh()
{
}

void StaticMesh::Destroy()
{
	Mesh::Destroy();
}

bool StaticMesh::Create(IDirect3DDevice9& device, sp<FBXMESHDATA> fbxData)
{
	assert(fbxData && "null reference: fbxData");

	_vertexCount = fbxData->verticeDataVec.size();
	_primitiveCount = fbxData->indiceDataVec.size();

	assert(_vertexCount && " does not exist vertice data");
	assert(_primitiveCount && " does not exist indice data");

	_vb = resourceManager.Create<VertexBuffer>();
	_ib = resourceManager.Create<IndexBuffer>();

	if (!_vb->CreateVertexBuffer(device, _vertexCount * sizeof(STATICMESHVERTEX), 0, 0, D3DPOOL_MANAGED))
		return false;

	if (!_ib->CreateIndexBuffer(device, _primitiveCount * sizeof(INDEX), 0, D3DPOOL_MANAGED))
		return false;

	if (!_vb->CreateVertexDeclaration(device, StaticMeshDec))
		return false;

	void * pData = nullptr;
	if (_vb->Lock(0, _vertexCount * sizeof(STATICMESHVERTEX), &pData, 0))
	{
		STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX*>(pData);
		STATICMESHVERTEX vertex;

		for (int i = 0; i < _vertexCount; ++i)
		{
			vertex.position = fbxData->verticeDataVec[i].vertex.position;
			vertex.color = fbxData->verticeDataVec[i].vertex.color;
			vertex.normal = fbxData->verticeDataVec[i].vertex.normal;
			vertex.texCoord = fbxData->verticeDataVec[i].vertex.texCoord;
			vertex.tangent = fbxData->verticeDataVec[i].vertex.tangent;

			*pVertice++ = vertex;
		}

		_vb->Unlock();
	}

	pData = nullptr;
	if (_ib->Lock(0, _primitiveCount * sizeof(INDEX), &pData, 0))
	{
		INDEX* pIndice = static_cast<INDEX*>(pData);
		INDEX index;

		for (int i = 0; i < _primitiveCount;)
		{
			for (int j = 0; j < 3; j++)
			{
				index._0 = fbxData->indiceDataVec[i++];
				index._1 = fbxData->indiceDataVec[i++];
				index._2 = fbxData->indiceDataVec[i++];
				*pIndice++ = index;
			}						
		}

		_ib->Unlock();
	}

	return true;
}
