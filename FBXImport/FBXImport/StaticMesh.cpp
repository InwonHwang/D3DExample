#include "StaticMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

StaticMesh::StaticMesh(IDirect3DDevice9& device)
	: Mesh(device)
{	
	_isSkinned = false;
	_vb = new VertexBuffer(device);
	_ib = new IndexBuffer(device);	
}


StaticMesh::~StaticMesh()
{	
	SAFE_DELETE(_vb);
	SAFE_DELETE(_ib);
}


// inherited from IMemento
void StaticMesh::Load(FbxNode& fbxNode, void* pData)
{
	LoadVerticeAndIndice(fbxNode, pData);
}

void StaticMesh::Save(FbxNode& fbxNode)
{

}

// inherited from Mesh
void StaticMesh::Draw()
{
	assert(_vb->GetVertexBuffer() && "vertex buffer doesn't created!!");
	assert(_ib->GetData() && "index buffer doesn't created!!");

	_device->SetStreamSource(0, _vb->GetVertexBuffer(), 0, sizeof(MESHVERTEX));
	_device->SetVertexDeclaration(_vb->GetVertexDeclaration());
	_device->SetIndices(_ib->GetData());
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertexCount, 0, _primitiveCount);

}

void StaticMesh::LoadVerticeAndIndice(FbxNode& fbxNode, void* pData)
{
	FbxMesh* pMesh = fbxNode.GetMesh();

	std::vector<MESHVERTEX>		bufferVertice;
	std::vector<unsigned long>	bufferIndice;

	int index = 0;	
	int polygonCount = pMesh->GetPolygonCount();
	
	for (int i = 0; i < polygonCount; i++)
	{
		int numVertices = pMesh->GetPolygonSize(i);
	
		for (int j = 0; j < numVertices; j++)			// numVertices�� 3���� 4���� �� ���� ���� -> size�� �� �� ����. vector���� array�� ������� ���Ѵ�.
		{
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
	
			MESHVERTEX vertex;
			vertex.Position.x = (float)pMesh->GetControlPointAt(controlPointIndex).mData[0];
			vertex.Position.y = (float)pMesh->GetControlPointAt(controlPointIndex).mData[1];
			vertex.Position.z = (float)pMesh->GetControlPointAt(controlPointIndex).mData[2];
	
			FbxVector4 normal;
			FbxVector2 uv;
			bool unmapped;
			
			pMesh->GetPolygonVertexNormal(i, j, normal);
			pMesh->GetPolygonVertexUV(i, j, 0, uv, unmapped);
			vertex.Normal.x = (float)normal.mData[0];
			vertex.Normal.y = (float)normal.mData[1];
			vertex.Normal.z = (float)normal.mData[2];
			vertex.TexCoord.x = (float)uv.mData[0];
			vertex.TexCoord.y = (float)uv.mData[1];
	
			bufferVertice.push_back(vertex);
			bufferIndice.push_back(index++);
		}
	}

	_vertexCount = bufferVertice.size();
	_primitiveCount = bufferIndice.size() / 3;

	const D3DVERTEXELEMENT9 dec[4] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,0 },
		D3DDECL_END()
	};

	int bufferVertexSize = _vertexCount * sizeof(MESHVERTEX);
	if (bufferVertexSize > 0)					// CreateVertexBuffer�� �� ����� 0�̸� ������ ��.
	{
		_vb->Create(bufferVertexSize, 0, D3DPOOL_MANAGED);
		_vb->CreateVertexDeclaration(dec);

		void* pData;

		if (_vb->Lock(0, bufferVertexSize, &pData, 0))
		{
			memcpy(pData, &bufferVertice[0], bufferVertexSize);

			_vb->Unlock();
		}
	}

	// 16bit �ε����� ������ 32bit �ε����� ������ Device caps�� ���ؼ� �˾Ƴ���
	int bufferIndiceSize = bufferIndice.size() * sizeof(unsigned long);
	if (bufferIndiceSize > 0)
	{
		_ib->Create(bufferIndiceSize, 0, D3DPOOL_MANAGED);
		void * pData;
		if (_ib->Lock(0, bufferIndiceSize * sizeof(unsigned long), &pData, 0))
		{
			memcpy(pData, &bufferIndice[0], bufferIndiceSize);

			_ib->Unlock();
		}
	}

}
