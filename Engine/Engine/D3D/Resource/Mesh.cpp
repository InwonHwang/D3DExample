#include "Mesh.h"

extern ResourceManager resourceManager;

Mesh::Mesh(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool),
	_vb(nullptr),
	_ib(nullptr),
	_isSkinned(nullptr),
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
	_isSkinned = false;
	_vertexCount = 0;
	_primitiveCount = 0;
}

bool Mesh::Create(IDirect3DDevice9& device, sp<FBXMeshData> fbxData)
{
	assert(fbxData && "null reference: fbxData");

	_vertexCount = fbxData->_positionVec.size();
	_primitiveCount = fbxData->_indiceVec.size();

	assert(_vertexCount && " does not exist vertice data");
	assert(_primitiveCount && " does not exist indice data");

	_vb = resourceManager.Create<VertexBuffer>();
	_ib = resourceManager.Create<IndexBuffer>();

	if (!_vb->CreateVertexBuffer(device, _vertexCount * sizeof(STATICMESHVERTEX), 0, D3DPOOL_MANAGED))
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
			vertex.position = fbxData->_positionVec[i];
			vertex.color = fbxData->_colorVec[i];
			vertex.normal = fbxData->_normalVec[i];
			vertex.texCoord = fbxData->_texCroodVec[i];
			vertex.tangent = fbxData->_tangentVec[i];

			*pVertice++ = vertex;
		}

		_vb->Unlock();
	}

	pData = nullptr;
	if (_ib->Lock(0, _primitiveCount * sizeof(INDEX), &pData, 0))
	{
		INDEX* pIndice = static_cast<INDEX*>(pData);
		INDEX index;

		for (int i = 0; i < _primitiveCount; ++i)
		{
			index = fbxData->_indiceVec[i];

			*pIndice++ = index;
		}

		_ib->Unlock();
	}

	return true;
}