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
		// vertex�� �ٽ� ���� ������
		// �ƴϸ� nullptr ���·� ó���Ұ�����
		// ȣ���ϴ� �Լ����� ó���ϱ�		
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
		// Lock�� �ɶ����� �Ұ�����
		// �����Ϸ� �Ұ�����
		// ȣ���ϴ� �Լ����� ó���ϱ�
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
