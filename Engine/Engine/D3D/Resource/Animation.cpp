#include "Animation.h"



Animation::Animation(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
	_animSetVec.reset(new AnimSetVec);
}


Animation::~Animation()
{
}

void Animation::Destroy()
{
	_animSetVec->erase(_animSetVec->begin(), _animSetVec->end());
}

bool Animation::Create(std::vector<sp<FBXBONEDATA>> fbxBoneDataVec)
{	
	sp<ANIMATIONKEYSET> pAnimKeySet;

	for (auto boneData : fbxBoneDataVec)
	{
		pAnimKeySet.reset(new ANIMATIONKEYSET);

		pAnimKeySet->start = 0;
		pAnimKeySet->end = boneData->end - boneData->start;
		pAnimKeySet->length = pAnimKeySet->end - pAnimKeySet->start;
		pAnimKeySet->name = boneData->name;		

		KEYFRAME* temp = boneData->pAnimation;
		while (temp)
		{
			FbxAMatrix tempMatrix = boneData->pAnimation->globalTransform;
			int frame = static_cast<int>(boneData->pAnimation->frameCount) - boneData->start;

			ANIMATIONKEY scale, rotation, position;
			scale.frame = frame;
			scale.value = FbxDXUtil::ToVector3(tempMatrix.GetS());
			rotation.frame = frame;
			rotation.value = FbxDXUtil::ToVector3(tempMatrix.GetR());
			position.frame = frame;
			position.value = FbxDXUtil::ToVector3(tempMatrix.GetT());
			
			pAnimKeySet->scale.push_back(scale);
			pAnimKeySet->rotation.push_back(rotation);
			pAnimKeySet->position.push_back(position);

			temp = temp->pNext;
		}		

		_animSetVec->push_back(pAnimKeySet);
	}

	return true;
}
