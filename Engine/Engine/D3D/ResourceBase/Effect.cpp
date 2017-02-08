#include "Effect.h"

Effect::Effect(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool),
	_effect(nullptr)
{
}

Effect::~Effect()
{
}

void Effect::Destroy()
{
	SafeRelease<ID3DXEffect>(_effect);
}

bool Effect::CreateEffect(IDirect3DDevice9& device, const String& fileName)
{
	SafeRelease<ID3DXEffect>(_effect);

	ID3DXBuffer* pError = nullptr;	
	DWORD flags = 0;

#if _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

#ifdef UNICODE 
	D3DXCreateEffectFromFile(&device, fileName.c_str(), NULL, NULL, flags, NULL, &_effect, &pError);
#elif
	D3DXCreateEffectFromFileA(&device, fileName.c_str(), NULL, NULL, flags, NULL, &_effect, &pError);
#endif	

	// ���̴� �ε��� ������ ��� outputâ�� ���̴�
	// ������ ������ ����Ѵ�.
	if (!_effect && pError)
	{
		//int size = pError->GetBufferSize();
		//void *ack = pError->GetBufferPointer();

		return false;
	}
	

	return true;
}