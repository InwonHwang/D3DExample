#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ResourceID handle, ResourceTable* pool)
	: ResourceItem(handle, pool),
	_ib(nullptr)
{
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Destroy()
{
	SafeRelease<IDirect3DIndexBuffer9>(_ib);
	ResourceItem::Unload();
	ResourceItem::Clear();
}

bool IndexBuffer::CreateIndexBuffer(IDirect3DDevice9& device, int size, DWORD usage, D3DPOOL pool)
{
	SafeRelease<IDirect3DIndexBuffer9>(_ib);

	HRESULT hr = 0;

#ifdef _USE_INDEX16
	if (FAILED(hr = device.CreateIndexBuffer(size, usage, D3DFMT_INDEX16, pool, &_ib, 0)))
#else
	if (FAILED(hr = device.CreateIndexBuffer(size, usage, D3DFMT_INDEX32, pool, &_ib, 0)))
#endif
	{
		Debug::D3DError(hr);
		return false;
	}	

	if (IsBackup()) // 백업 옵션을 갖고 있으면 힙에 백업 데이터 저장
	{
	}

	return true;
}

bool IndexBuffer::Lock(int offset, int size, void** ppData, DWORD flags)
{
	if(!_ib) return false;

	HRESULT hr = 0;
	if (FAILED(hr = _ib->Lock(offset, size, ppData, flags)))
	{
		Debug::D3DError(hr);
		return false;
	}

	return true;
}

bool IndexBuffer::Unlock()
{
	if (!_ib) return false;

	HRESULT hr = 0;
	if (FAILED(hr = _ib->Unlock()))
	{
		Debug::D3DError(hr);
		return false;
	}

	return true;;
}