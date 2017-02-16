#pragma once

#include "..\..\Core\Core.h"
#include "..\D3DDescription\D3DDescription.h"

class Animation : public ResourceItem
{
	typedef std::vector<sp<ANIMATIONKEYSET>> AnimSetVec;
public:
	Animation(ResourceHandle handle, ResourcePoolImpl* pool);
	~Animation();

	virtual void Destroy() override;

	bool Create(std::vector<sp<FBXBONEDATA>> fbxBoneDataVec);

	sp<AnimSetVec> GetAnimSetVec() const;

private:
	sp<AnimSetVec> _animSetVec;
};

inline sp<Animation::AnimSetVec> Animation::GetAnimSetVec() const
{
	return _animSetVec;
}

