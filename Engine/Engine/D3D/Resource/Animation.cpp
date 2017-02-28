#include "Animation.h"



Animation::Animation(ResourceID handle, ResourceTable* pool)
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
	sp<KEYFRAME> pKeyFrame;
	Vector3 v(0, 0, 0);
	Quaternion q(0, 0, 0, 0);
	
	for (auto boneData : fbxBoneDataVec)
	{
		pAnimCurve.reset(new AnimationCurve);
		pAnimCurve->SetBoneName(boneData->name);
		

		for (uint i = 0; i < boneData->animVec.size(); ++i)
		{
			pKeyFrame = boneData->animVec[i];

			FbxAMatrix tempMatrix = pKeyFrame->globalTransform;
			int frame = (static_cast<int>(pKeyFrame->frame) - boneData->start) * 60;

			v = FbxDXUtil::ToVector3(tempMatrix.GetS());			
			pAnimCurve->SetScaleKey(frame, v);

			q = FbxDXUtil::ToQuaternion(tempMatrix.GetQ());
			pAnimCurve->SetRotationKey(frame, q);

			v = FbxDXUtil::ToVector3(tempMatrix.GetT());
			pAnimCurve->SetPositionKey(frame, v);
		}
	

		pAnimCurve->SetFrame();

		_animCurveVec->push_back(pAnimCurve);
	}

	return true;
}
