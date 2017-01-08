#pragma once
#include "IMemento.h"
#include "Core.h"

class IndexBuffer
{
public:
	IndexBuffer(IDirect3DDevice9& device);
	~IndexBuffer();

	bool	Create(int size, DWORD usage, D3DPOOL pool);

	bool	Lock(int offset, int size, void** ppData, DWORD flags);
	bool	Unlock();

	IDirect3DIndexBuffer9* IndexBuffer::GetData() const;

private:
	IDirect3DDevice9*			_device;
	IDirect3DIndexBuffer9*		_ib;
};


inline IDirect3DIndexBuffer9* IndexBuffer::GetData() const
{
	return _ib;
}