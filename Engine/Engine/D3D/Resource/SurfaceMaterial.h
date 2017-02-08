#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"

class Effect;

class SurfaceMaterial : public ResourceItem
{
	typedef std::vector<D3DXPARAMETER_DESC> ParamEffectVec;

public:
	SurfaceMaterial(ResourceHandle handle, ResourcePoolImpl* pool);
	~SurfaceMaterial();

	virtual void Destroy() override;

	void SetEffect(const sp<Effect> effect);

	void SetFloat(const String& semantic, float value);
	void SetInt(const String& semantic, int value);
	void SetMatrix(const String& semantic, const D3DXMATRIX& matrix);
	void SetTexture(const String& semantic, const sp<Texture> texture);

	uint GetParamCount() const;							// effect 파일 안에 parameter 개수
	String GetSementic(uint index) const;
	D3DXPARAMETER_TYPE GetParamType(uint index) const;	// paramete 타입 정보	

	void Begin(uint& numPass);
	void End();
	void BeginPass(uint i);
	void EndPass();

private:
	sp<ParamEffectVec>	_paramEffect;
	sp<Effect>			_effect;
	
};

inline void SurfaceMaterial::SetFloat(const String& semantic, float value)
{
	assert(_effect && "null reference : _effect");	

#ifdef _UNICODE
	std::string str;
	str.assign(semantic.begin(), semantic.end());
	_effect->GetD3DEffect()->SetFloat(str.c_str(), value);
#else
	_effect->GetD3DEffect()->SetFloat(semantic.c_str(), value);
#endif	
}

inline void SurfaceMaterial::SetInt(const String& semantic, int value)
{
	assert(_effect && "null reference : _effect");

#ifdef _UNICODE
	std::string str;
	str.assign(semantic.begin(), semantic.end());
	_effect->GetD3DEffect()->SetInt(str.c_str(), value);
#else
	_effect->GetD3DEffect()->SetInt(semantic.c_str(), value);
#endif	
}

inline void SurfaceMaterial::SetMatrix(const String& semantic, const D3DXMATRIX& matrix)
{
	assert(_effect && "null reference : _effect");

#ifdef _UNICODE
	std::string str;
	str.assign(semantic.begin(), semantic.end());
	_effect->GetD3DEffect()->SetMatrix(str.c_str(), &matrix);
#else
	_effect->GetD3DEffect()->SetMatrix(semantic.c_str(), &matrix);
#endif	
}

inline void SurfaceMaterial::SetTexture(const String& semantic, const sp<Texture> texture)
{
	assert(_effect && "null reference : _effect");
#ifdef _UNICODE
	std::string str;
	str.assign(semantic.begin(), semantic.end());
	_effect->GetD3DEffect()->SetTexture(str.c_str(), texture->GetD3DTexture());
#else
	_effect->GetD3DEffect()->SetTexture(semantic.c_str(), texture->GetD3DTexture());
#endif
}

inline uint SurfaceMaterial::GetParamCount() const
{
	assert(_paramEffect && "null reference : _paramEffect");
	return _paramEffect->size();
}

inline String SurfaceMaterial::GetSementic(uint index) const
{
	assert(_paramEffect && "null reference : _paramEffect");
	
	std::string str = _paramEffect->data()[index].Semantic;

#ifdef _UNICODE
	String ret;
	ret.assign(str.begin(), str.end());		
	return ret;
#else
	return str;
#endif
}

inline D3DXPARAMETER_TYPE SurfaceMaterial::GetParamType(uint index) const
{
	assert(_paramEffect && "null reference : _paramEffect");
	return _paramEffect->data()[index].Type;
}