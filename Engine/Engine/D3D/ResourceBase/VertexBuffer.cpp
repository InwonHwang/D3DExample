#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(ResourceID handle, ResourceTable* pool)
	: ResourceItem(handle, pool),
	_vb(nullptr),
	_decVertex(nullptr)
{
}


VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::Destroy()
{
	SafeRelease<IDirect3DVertexBuffer9>(_vb);
	SafeRelease<IDirect3DVertexDeclaration9>(_decVertex);
	ResourceItem::Unload();
	ResourceItem::Clear();
}

bool VertexBuffer::CreateVertexBuffer(IDirect3DDevice9& device, int size, DWORD usage, DWORD fvf, D3DPOOL pool)
{
	SafeRelease<IDirect3DVertexBuffer9>(_vb);

	HRESULT hr = 0;
	if (FAILED(hr = device.CreateVertexBuffer(size, usage, (D3DFVF_XYZ | D3DFVF_TEX3 |
		D3DFVF_TEXCOORDSIZE2(0) |		// texture
		D3DFVF_TEXCOORDSIZE4(1) |		// blend weight
		D3DFVF_TEXCOORDSIZE4(2)), pool, &_vb, NULL)))
	{
		DebugError(hr);
		return false;
	}

	if (IsBackup()) // 백업 옵션을 갖고 있으면 힙에 백업 데이터 저장
	{
	}
	
	return true;
}

bool VertexBuffer::CreateVertexDeclaration(IDirect3DDevice9& device, const D3DVERTEXELEMENT9* vertexElement)
{
	SafeRelease<IDirect3DVertexDeclaration9>(_decVertex);

	HRESULT hr = 0;
	if (FAILED(device.CreateVertexDeclaration(vertexElement, &_decVertex)))
	{
		DebugError(hr);
		return false;
	}

	return true;
}

bool VertexBuffer::Lock(int offset, int size, void** ppData, DWORD flags)
{
	assert(_vb && "null reference: _vb");

	HRESULT hr = 0;
	if (FAILED(hr = _vb->Lock(offset, size, ppData, flags)))
	{
		DebugError(hr);
		return false;
	}

	return true;
}

bool VertexBuffer::Unlock()
{
	assert(_vb && "null reference: _vb");

	HRESULT hr = 0;
	if (FAILED(hr = _vb->Unlock()))
	{
		DebugError(hr);
		return false;
	}

	return true;
}