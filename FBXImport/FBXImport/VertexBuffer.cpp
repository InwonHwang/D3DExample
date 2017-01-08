#include "VertexBuffer.h"
#include "VertexDescription.h"

VertexBuffer::VertexBuffer(IDirect3DDevice9& device)
	: _device(&device),
	_vb(nullptr),
	_decVertex(nullptr)
{
}


VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(_vb);
}



bool VertexBuffer::Create(int size, DWORD usage, D3DPOOL pool)
{
	assert(!_vb && "vertex buffer alreay created!!");

	HRESULT hr = 0;
	if (FAILED(hr = _device->CreateVertexBuffer(size, usage, 0, pool, &_vb, NULL)))
	{
		// vertex를 다시 만들 것인지
		// 아니면 nullptr 상태로 처리할것인지
		// 호출하는 함수에서 처리하기		
		DebugError(hr);
		return false;
	}

	return true;
}

bool VertexBuffer::Lock(int offset, int size, void** ppData, DWORD flags)
{
	HRESULT hr = 0;
	if (FAILED(hr = _vb->Lock(offset, size, ppData, flags)))
	{	
		// Lock을 될때까지 할것인지
		// 없던일로 할것인지
		// 호출하는 함수에서 처리하기
		DebugError(hr);
		return false;
	}

	return true;
}

bool VertexBuffer::Unlock()
{
	HRESULT hr = 0;
	if (FAILED(hr = _vb->Unlock()))
	{
		DebugError(hr);
		return false;
	}

	return true;
}

bool  VertexBuffer::CreateVertexDeclaration(const D3DVERTEXELEMENT9* vertexElement)
{
	HRESULT hr = 0;
	if (FAILED(_device->CreateVertexDeclaration(vertexElement, &_decVertex)))
	{
		DebugError(hr);
		return false;
	}

	return true;
}
