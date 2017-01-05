#include "VertexBuffer.h"
#include "VertexDescription.h"

VertexBuffer::VertexBuffer()
	: _data(nullptr),
	_byteSize(0),
	_usage(0)
{
}


VertexBuffer::~VertexBuffer()
{
}



bool VertexBuffer::Create(IDirect3DDevice9& device, int size, int stride, DWORD usage, DWORD FVF, D3DPOOL pool)
{
	assert(_data == nullptr && "vertex buffer alreay created!!");

	_byteSize = size * stride;
	_usage = usage;	

	if (FAILED(device.CreateVertexBuffer(_byteSize, _usage, FVF, pool, &_data, NULL)))
	{
		// vertex�� �ٽ� ���� ������
		// �ƴϸ� nullptr ���·� ó���Ұ�����
		// ȣ���ϴ� �Լ����� ó���ϱ�		
		return false;
	}

	return true;
}

void VertexBuffer::Destroy()
{
	if (_data) _data->Release();
}

bool VertexBuffer::Lock(int offset, int size, void** ppData, DWORD flags)
{
	if (FAILED(_data->Lock(offset, size, ppData, flags)))
	{
		// Lock�� �ɶ����� �Ұ�����
		// �����Ϸ� �Ұ�����
		// ȣ���ϴ� �Լ����� ó���ϱ�
		return false;
	}

	return true;
}

bool VertexBuffer::Unlock()
{
	if (FAILED(_data->Unlock()))
	{
		// unlock �ɶ����� �ؾߵ�.
		return false;
	}

	return true;
}

void VertexBuffer::Load(FbxNode& fbxNode) // mesh Node�� ��������
{
	assert(_data && "vertex buffer doesn't created!!");

	FbxMesh* pMesh = (FbxMesh*)fbxNode.GetNodeAttribute();

	FbxVector4* pVertices = pMesh->GetControlPoints();

	void* pData;
	if (Lock(0, _byteSize, (void**)&pData, _usage))
	{
		MeshVertex* pMeshVertice = static_cast<MeshVertex*>(pData);
		int polygonCount = pMesh->GetPolygonCount();
		

		for (int i = 0; i < polygonCount; i++)
		{
			int numVertices = pMesh->GetPolygonSize(i);

			for (int j = 0; j < numVertices; j++)
			{
				int controlPointIndex = pMesh->GetPolygonVertex(i, j);

				FbxVector4 normal;
				FbxVector2 uv;
				bool unmapped;
				
				pMesh->GetPolygonVertexNormal(i, j, normal);
				pMesh->GetPolygonVertexUV(i, j, 0, uv, unmapped);

				MESHVERTEX vertex;
				vertex.Position.x = (float)pVertices[controlPointIndex].mData[0];
				vertex.Position.y = (float)pVertices[controlPointIndex].mData[1];
				vertex.Position.z = (float)pVertices[controlPointIndex].mData[2];
				vertex.Normal.x = (float)normal.mData[0];
				vertex.Normal.x = (float)normal.mData[1];
				vertex.Normal.x = (float)normal.mData[2];
				vertex.UV.x = (float)uv.mData[0];
				vertex.UV.x = (float)uv.mData[1];


				*pMeshVertice++ = vertex;
			}

		}
		Unlock();
	}
}

void VertexBuffer::Save(FbxNode& fbxNode)
{

}

void* VertexBuffer::GetData()
{
	return _data;
}
