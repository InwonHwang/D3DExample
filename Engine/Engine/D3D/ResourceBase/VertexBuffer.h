#pragma once
#include "..\..\Core\Core.h"

class VertexBuffer : public ResourceItem
{
public:
	VertexBuffer(ResourceID handle, ResourceTable* pool);
	~VertexBuffer();

	virtual void Destroy() override;
		
	bool CreateVertexBuffer(IDirect3DDevice9& device, int size, DWORD usage, DWORD fvf, D3DPOOL pool);
	bool CreateVertexDeclaration(IDirect3DDevice9& device, const D3DVERTEXELEMENT9* vertexElement);

	bool Lock(int offset, int size, void** ppData, DWORD flags);
	bool Unlock();

	IDirect3DVertexBuffer9*  GetD3DVertexBuffer() const;
	IDirect3DVertexDeclaration9* GetD3DVertexDeclaration() const;

private:
	IDirect3DVertexBuffer9*			_vb;
	IDirect3DVertexDeclaration9*	_decVertex;
};

inline IDirect3DVertexBuffer9*  VertexBuffer::GetD3DVertexBuffer() const
{
	return _vb;
}

inline IDirect3DVertexDeclaration9* VertexBuffer::GetD3DVertexDeclaration() const
{
	return _decVertex;
}
