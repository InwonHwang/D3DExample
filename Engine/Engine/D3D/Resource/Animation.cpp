#include "Animation.h"



Animation::Animation(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
	_animCurveVec.reset(new AnimCurveVec);
}


Animation::~Animation()
{
}

void Animation::Destroy()
{
	_animCurveVec->erase(_animCurveVec->begin(), _animCurveVec->end());
}

bool Animation::Create(std::vector<sp<FBXBONEDATA>> fbxBoneDataVec)
{	
	sp<AnimationCurve> pAnimCurve;
	
	for (auto boneData : fbxBoneDataVec)
	{
		sp<KEYFRAME> temp;

		pAnimCurve.reset(new AnimationCurve);
		pAnimCurve->SetBoneName(boneData->name);

		for (uint i = 0; i < boneData->animVec.size(); ++i)
		{
			temp = boneData->animVec[i];

			FbxAMatrix tempMatrix = temp->globalTransform;
			int frame = (static_cast<int>(temp->frame) - boneData->start) * 60;

			Vector3 value;

			value = FbxDXUtil::ToVector3(tempMatrix.GetS());
			pAnimCurve->SetScaleKey(frame, value);

			value = FbxDXUtil::ToVector3(tempMatrix.GetR());
			pAnimCurve->SetRotationKey(frame, value);

			value = FbxDXUtil::ToVector3(tempMatrix.GetT());
			pAnimCurve->SetPositionKey(frame, value);
		}
	

		pAnimCurve->SetFrame();

		_animCurveVec->push_back(pAnimCurve);
	}

	return true;
}
