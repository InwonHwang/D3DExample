#pragma once
#include "IMemento.h"
#include "Core.h"
#include "Description.h"

class VertexBuffer
{
public:
	VertexBuffer(IDirect3DDevice9& device);
	~VertexBuffer();
	
	bool Create(int size, DWORD usage, D3DPOOL pool);	

	bool Lock(int offset, int size, void** ppData, DWORD flags);
	bool Unlock();

	bool CreateVertexDeclaration(const D3DVERTEXELEMENT9* vertexElement);

	IDirect3DVertexBuffer9*  GetVertexBuffer() const;
	IDirect3DVertexDeclaration9* GetVertexDeclaration() const;

private:
	IDirect3DDevice9*				_device;	
	IDirect3DVertexBuffer9*			_vb;
	IDirect3DVertexDeclaration9*	_decVertex;
};

inline IDirect3DVertexBuffer9* VertexBuffer::GetVertexBuffer() const
{
	return _vb;
}

inline IDirect3DVertexDeclaration9* VertexBuffer::GetVertexDeclaration() const
{
	return _decVertex;
}

