#include "SurfaceMaterial.h"



SurfaceMaterial::SurfaceMaterial(ResourceID handle, ResourceTable* pool)
	: ResourceItem(handle, pool),
	_paramEffect(nullptr),
	_effect(nullptr)
{
	_paramEffect.reset(new ParamEffectVec);
}


SurfaceMaterial::~SurfaceMaterial()
{
}

void SurfaceMaterial::Destroy()
{
	_paramEffect->clear();
	_paramEffect.reset();
	_effect.reset();
}


void SurfaceMaterial::SetEffect(const sp<Effect> effect)
{
	_effect = effect;
	_paramEffect->clear();

	assert(_effect->GetD3DEffect());

	D3DXEFFECT_DESC effectDesc = { 0, };

	_effect->GetD3DEffect()->GetDesc(&effectDesc);
	
	// paramEffect �� ä���
	for (uint i = 0; i < effectDesc.Parameters; ++i)	// D3DXPARAMETER_DESC ���� ��ŭ
	{ 
		D3DXHANDLE handle = NULL;
		D3DXPARAMETER_DESC paramDesc = { 0, };

		handle = _effect->GetD3DEffect()->GetParameter(NULL, i);		// D3DXPARAMETER_DESC�� ���� �ڵ� ���
		_effect->GetD3DEffect()->GetParameterDesc(handle, &paramDesc);	// D3DXPARAMETER_DESC ���

		if (!paramDesc.Semantic) continue;		// �ø�ƽ�� ���� ����Ʈ ������ ä����� �ϱ� ������ �ø�ƽ�� ������ ����

		_paramEffect->push_back(paramDesc);
	}
}

void SurfaceMaterial::Begin(uint& numPass)
{
	assert(_effect && "null reference: _effect");
	_effect->GetD3DEffect()->Begin(&numPass, 0);
}
void SurfaceMaterial::End()
{
	assert(_effect && "null reference: _effect");
	_effect->GetD3DEffect()->End();
}
void SurfaceMaterial::BeginPass(uint i)
{
	assert(_effect && "null reference: _effect");
	_effect->GetD3DEffect()->BeginPass(i);
}
void SurfaceMaterial::EndPass()
{
	assert(_effect && "null reference: _effect");
	_effect->GetD3DEffect()->EndPass();
}
