#pragma once
#include "IMemento.h"
#include "Core.h"
#include <d3dx9.h>
#include <d3d9.h>

class VertexBuffer : public IMemento
{
public:
	VertexBuffer();
	~VertexBuffer();

	bool Create(IDirect3DDevice9& device, int size, int stride, DWORD usage, DWORD FVF, D3DPOOL pool);
	void Destroy();

	bool Lock(int offset, int size, void** ppData, DWORD flags);
	bool Unlock();

	virtual void Load(FbxNode& fbxNode) override;
	virtual void Save(FbxNode& fbxNode) override;
	virtual void* GetData() override;	

private:
	IDirect3DVertexBuffer9* _data;
	int _byteSize;					// size * stride Load�� ���� ����
	DWORD _usage;					// ���ؽ� ���� �뵵 Load�� ���� ����
									// �ǽð����� ���ؽ� ������ �뵵�� �ٲ��� enum�� �̿��ؼ� ó���ؾ���.
};

