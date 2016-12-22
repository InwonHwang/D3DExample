#pragma once
#include "../Core/Core.h"
#include <d3d9.h>

class VertexBuffer
{
public:\
	enum LockType {
		DYNAMIC,		// true ? D3DUSAGE_DYNIMIC , D3DPOOL_DEFAULT : D3DPOOL_MANAGED
		OVERWRITE,		// true ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE 
		POINT,			// D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY
	};

private:
	IDirect3DVertexBuffer9* _d3dVB;
	flags32 _flags;
	uint _lockFlags;
	uint _usageFlags;

	bool create(flags32 flags, void* pData);

public:
	VertexBuffer() {}
	~VertexBuffer() {}
};

bool VertexBuffer::create(flags32 flags, void* pData)
{
	_flags = flags;
	_lockFlags = 0;
	_usageFlags = 0;
	D3DPOOL poolFlags = 0;

	if (_flags.check(DYNAMIC))
	{
		_usageFlags |= D3DUSAGE_DYNAMIC;
		poolFlags = D3DPOOL_DEFAULT;
	}
	else
	{
		poolFlags = D3DPOOL_MANAGED;
	}

	if (_flags.check(POINT))
	{
		_usageFlags |= D3DUSAGE_POINTS;
		_usageFlags |= D3DUSAGE_WRITEONLY;
	}

	
}
