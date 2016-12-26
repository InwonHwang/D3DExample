#include "IndexBuffer.h"
#include "../Device.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::create(UINT count, UINT indexSize, DWORD FVF, flags8 type, DWORD usage, void* pData)
{
	assert(_d3dIB);
	assert(_backupBuffer);

	_typeFlags = type;
	_usageFlags = usage;
	_size = count * indexSize;

	D3DPOOL poolFlags = _typeFlags.check(TYPE::DYNAMIC) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
	D3DFORMAT fmt;

#ifdef _USE_INDEX16
	fmt = D3DFMT_INDEX16;
#else
	fmt = D3DFMT_INDEX32;
#endif

	HRESULT hr = S_OK;
	if (FAILED(hr = D3DDevice->CreateIndexBuffer(_size,
		_usageFlags,
		fmt,
		poolFlags,
		&_d3dIB, 0)))
	{
		_d3dIB = 0;
		return false;
	}

	if (_typeFlags.check(TYPE::RAMBACKUP))
	{
		// _size 만큼 할당
	}

	create();
	return true;
}

void* IndexBuffer::lock(UINT offset, UINT size)
{
	assert(!_d3dIB);

	void* pData = nullptr;

	if (FAILED(_d3dIB->Lock(offset, size, &pData, _usageFlags)))
	{
		// 에러 출력
		if (_typeFlags.check(TYPE::RAMBACKUP))
		{

		}
	}
	return pData;
}

void IndexBuffer::unlock()
{
	assert(!_d3dIB);

	if (FAILED(_d3dIB->Unlock()))
	{
		// 에러 출력
	}
}

void IndexBuffer::create()
{
	notifyCreated();
}

void IndexBuffer::destroy()
{
	notifyDestroyed();

	SAFE_RELEASE(_d3dIB);
	SAFE_DELETE_ARRAY(_backupBuffer);
	_typeFlags.clear();
	_usageFlags = 0;
	_size = 0;
}

void IndexBuffer::disable()
{
	notifyDisabled();
	if (_typeFlags.check(TYPE::VOLITILE))
	{
		// release
	}
}

void IndexBuffer::restore()
{
	notifyRestored();
	if (_typeFlags.check(TYPE::RAMBACKUP))
	{
		// _backupBuffer에 있는 내용 _d3dVB에 저장
	}
}
