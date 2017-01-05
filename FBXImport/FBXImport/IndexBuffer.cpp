#include "IndexBuffer.h"
#include "IndexDescription.h"

IndexBuffer::IndexBuffer()
	: _data(nullptr)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::Create(IDirect3DDevice9& device, int size, int stride, DWORD usage, D3DPOOL pool)
{
	assert(_data == nullptr && "index buffer already created!");

	_byteSize = size * stride;
	_usage = usage;

#ifdef _USE_INDEX16
	if (FAILED(device.CreateIndexBuffer(_byteSize, _usage, D3DFMT_INDEX16, pool, &_data, 0)))
#else
	if (FAILED(device.CreateIndexBuffer(_byteSize, _usage, D3DFMT_INDEX32, pool, &_data, 0)))
#endif
	{
		return false;
	}


	return true;
}

void IndexBuffer::Destroy()
{
	if (_data) _data->Release();
}

bool IndexBuffer::Lock(int offset, int size, void** ppData, DWORD flags)
{
	if (FAILED(_data->Lock(offset, size, ppData, flags)))
	{
		return false;
	}

	return true;
}

bool IndexBuffer::Unlock()
{
	if (FAILED(_data->Unlock()))
	{
		return false;
	}

	return true;;
}

void IndexBuffer::Load(FbxNode& fbxNode)
{
	assert(_data && "vertex buffer doesn't created!!");

	FbxMesh* pMesh = (FbxMesh*)fbxNode.GetNodeAttribute();

	void* pData;
	if (Lock(0, _byteSize, (void**)&pData, _usage))
	{
		INDEX* pIndex = static_cast<INDEX*>(pData);
		int polygonCount = pMesh->GetPolygonCount();

		for (int i = 0; i < polygonCount; i++)
		{
			INDEX index;
			int polygonSize = pMesh->GetPolygonSize(i);


			index._0 = (DWORD)pMesh->GetPolygonVertex(i, 0);
			index._1 = (DWORD)pMesh->GetPolygonVertex(i, 1);
			index._2 = (DWORD)pMesh->GetPolygonVertex(i, 2);

			
			*pIndex++ = index;
		}
		Unlock();
	}
}

void IndexBuffer::Save(FbxNode& fbxNode)
{

}

void* IndexBuffer::GetData()
{
	return _data;
}
