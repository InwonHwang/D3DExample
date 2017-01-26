#pragma once
#include "..\..\Core\Core.h"

class IndexBuffer : public ResourceItem
{
public:
	IndexBuffer(ResourceHandle handle, ResourcePoolImpl* pool);
	~IndexBuffer();

	virtual void Destroy() override;

	bool Create(IDirect3DDevice9& device, int size, DWORD usage, D3DPOOL pool);

	bool Lock(int offset, int size, void** ppData, DWORD flags);
	bool Unlock();

	IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;

private:
	IDirect3DIndexBuffer9* _ib;
};

inline IDirect3DIndexBuffer9* IndexBuffer::GetD3DIndexBuffer() const
{
	return _ib;
}

