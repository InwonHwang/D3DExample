#pragma once
#include "..\..\Core\Core.h"

class Effect : public ResourceItem
{
public:
	Effect(ResourceHandle handle, ResourcePoolImpl* pool);
	~Effect();

	virtual void Destroy() override;

	bool CreateEffect(IDirect3DDevice9& device, const String& fileName);

	ID3DXEffect* GetD3DEffect() const;

private:
	ID3DXEffect*	_effect;
};

inline ID3DXEffect* Effect::GetD3DEffect() const
{
	return _effect;
}

