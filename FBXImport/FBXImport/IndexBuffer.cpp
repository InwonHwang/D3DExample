#include "IndexBuffer.h"
#include "IndexDescription.h"

IndexBuffer::IndexBuffer(IDirect3DDevice9& device)
	:
	_device(&device),
	_ib(nullptr)
{
}

IndexBuffer::~IndexBuffer()
{
	SAFE_RELEASE(_ib);
}

bool IndexBuffer::Create(int size, DWORD usage, D3DPOOL pool)
{
	assert(_ib == nullptr && "index buffer already created!");

#ifdef _USE_INDEX16
	if (FAILED(_device->CreateIndexBuffer(size, usage, D3DFMT_INDEX16, pool, &_ib, 0)))
#else
	if (FAILED(_device->CreateIndexBuffer(size, usage, D3DFMT_INDEX32, pool, &_ib, 0)))
#endif
	{
		return false;
	}

	return true;
}

bool IndexBuffer::Lock(int offset, int size, void** ppData, DWORD flags)
{
	if (FAILED(_ib->Lock(offset, size, ppData, flags)))
	{
		return false;
	}

	return true;
}

bool IndexBuffer::Unlock()
{
	if (FAILED(_ib->Unlock()))
	{
		return false;
	}

	return true;;
}