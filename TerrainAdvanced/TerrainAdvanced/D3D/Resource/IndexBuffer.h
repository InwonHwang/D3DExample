#pragma once

#include "../../Core/Core.h"
#include <d3d9.h>

class IndexBuffer : public ResourceItem
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool create(UINT count, UINT indexSize, DWORD FVF, flags8 type, DWORD usage, void* pData);
	void *lock(UINT offset, UINT size);
	void unlock();

	void create() override;
	void destroy() override;
	void disable() override;
	void restore() override;

	bool isDynamic() const;
	bool isVolitile() const;
private:
	IDirect3DIndexBuffer9* _d3dIB;
	flags8  _typeFlags;
	DWORD	_usageFlags;
	UINT	_size;
	void*   _backupBuffer;
};

inline bool IndexBuffer::isDynamic() const
{
	return _typeFlags.check(TYPE::DYNAMIC);
}

inline bool IndexBuffer::isVolitile() const
{
	return _typeFlags.check(TYPE::VOLITILE);
}

