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
	
	// paramEffect 값 채우기
	for (uint i = 0; i < effectDesc.Parameters; ++i)	// D3DXPARAMETER_DESC 개수 만큼
	{ 
		D3DXHANDLE handle = NULL;
		D3DXPARAMETER_DESC paramDesc = { 0, };

		handle = _effect->GetD3DEffect()->GetParameter(NULL, i);		// D3DXPARAMETER_DESC을 얻어올 핸들 얻기
		_effect->GetD3DEffect()->GetParameterDesc(handle, &paramDesc);	// D3DXPARAMETER_DESC 얻기

		if (!paramDesc.Semantic) continue;		// 시멘틱을 갖고 이펙트 정보를 채우려고 하기 때문에 시멘틱이 없으면 리턴

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
