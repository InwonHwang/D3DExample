#pragma once

#include "..\..\Core\Core.h"
#include "..\D3DStructure\D3DStructure.h"

class Animation : public ResourceItem
{
	typedef std::vector<sp<AnimationCurve>> AnimCurveVec;
public:
	Animation(ResourceHandle handle, ResourcePoolImpl* pool);
	~Animation();

	virtual void Destroy() override;

	bool Create(std::vector<sp<FBXBONEDATA>> fbxBoneDataVec);

	sp<AnimCurveVec> GetAnimCurveVec() { return _animCurveVec; }

private:
	sp<AnimCurveVec> _animCurveVec;
};


