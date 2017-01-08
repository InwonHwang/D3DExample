#include "StaticMesh.h"
#include "VertexBuffer.h"
#include "VertexDescription.h"
#include "IndexBuffer.h"
#include "IndexDescription.h"


StaticMesh::StaticMesh(IDirect3DDevice9& device)
	: Mesh(device)
{	
	_isSkinned = false;
	_vb = new VertexBuffer(device);
	_ib = new IndexBuffer(device);	
}


StaticMesh::~StaticMesh()
{	
	_bufferVertice.clear();
	_bufferIndice.clear();
	SAFE_DELETE(_vb);
	SAFE_DELETE(_ib);
}

void StaticMesh::Draw()
{
	assert(_vb->GetVertexBuffer() && "vertex buffer doesn't created!!");
	assert(_ib->GetData() && "index buffer doesn't created!!");	
		
	_device->SetStreamSource(0, _vb->GetVertexBuffer(), 0, sizeof(MESHVERTEX));
	_device->SetVertexDeclaration(_vb->GetVertexDeclaration());
	_device->SetIndices(_ib->GetData());
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertexCount, 0, _primitiveCount);
	
}

// inherited from IMemento
void StaticMesh::Load(FbxNode& fbxNode)
{
	InternalLoadVertice(fbxNode);
	InternalLoadIndice(fbxNode);	
	InternalSetUpVertexBuffer();
	InternalSetUpIndexBuffer();		
}

void StaticMesh::Save(FbxNode& fbxNode)
{

}

void* StaticMesh::GetData()
{
	return nullptr;
}

void StaticMesh::InternalLoadVertice(FbxNode& fbxNode)
{
	assert(_bufferVertice.size() == 0 && "vertex already loaded!!");
	if (_bufferVertice.size() > 0)	return;							// 실제로 사용자가 메쉬를 만들어낼때 이미 내부 데이터가 있으면 아무 동작 하지 않음.	

	FbxMesh* pMesh = fbxNode.GetMesh();
	
	int polygonCount = pMesh->GetPolygonCount();
	
	for (int i = 0; i < polygonCount; i++)
	{
		int numVertices = pMesh->GetPolygonSize(i);
	
		for (int j = 0; j < numVertices; j++)			// numVertices가 3인지 4일지 알 수가 없다 -> size를 알 수 없다. vector말고 array를 사용하지 못한다.
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
	
			_bufferVertice.push_back(vertex);
		}
	}	
	_vertexCount = _bufferVertice.size();
}

void StaticMesh::InternalLoadIndice(FbxNode& fbxNode)
{
	assert(_bufferIndice.size() == 0 && "index already loaded!!");
	if (_bufferIndice.size() > 0) return;

	FbxMesh* pMesh = fbxNode.GetMesh();
	int index = 0;

	int polygonCount = pMesh->GetPolygonCount();

	for (int i = 0; i < polygonCount; i++)
	{
		int polygonSize = pMesh->GetPolygonSize(i);

		for (int j = 0; j < polygonSize; j++)
		{
			_bufferIndice.push_back(index++);
		}
	}
	_primitiveCount = _bufferIndice.size() / 3;
}

void StaticMesh::InternalSetUpVertexBuffer()
{	
	const D3DVERTEXELEMENT9 dec[6] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,0 },
		D3DDECL_END()
	};	

	int bufferSize = _vertexCount * sizeof(MESHVERTEX);
	if (bufferSize > 0)					// CreateVertexBuffer일 때 사이즈가 0이면 에러가 뜸.
	{
		_vb->Create(bufferSize, 0, D3DPOOL_MANAGED);
		_vb->CreateVertexDeclaration(dec);

		void* pData;

		if (_vb->Lock(0, bufferSize, &pData, 0))
		{
			memcpy(pData, &_bufferVertice[0], bufferSize);

			_vb->Unlock();
		}
	}
}

void StaticMesh::InternalSetUpIndexBuffer()
{
	// 16bit 인덱스를 쓸건지 32bit 인덱스를 쓸건지 Device caps를 통해서 알아내기
	int bufferSize = _bufferIndice.size() * sizeof(unsigned long);
	if (bufferSize > 0)
	{
		_ib->Create(bufferSize, 0, D3DPOOL_MANAGED);
		void * pData;
		if (_ib->Lock(0, bufferSize * sizeof(unsigned long), &pData, 0))
		{
			memcpy(pData, &_bufferIndice[0], bufferSize);

			_ib->Unlock();
		}
	}	
}