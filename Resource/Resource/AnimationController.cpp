#include "AnimationController.h"
#include "Animation.h"
#include "Transform.h"
#include "Frame.h"

AnimationController::AnimationController(UINT maxNumMatrices, UINT maxNumAnimations, UINT maxNumTracks, UINT maxNumEvents)
	: _animationController(NULL)
{
	D3DXCreateAnimationController(maxNumMatrices, maxNumAnimations, maxNumTracks, maxNumEvents, &_animationController);
}


AnimationController::~AnimationController()
{
	SAFE_RELEASE(_animationController);
}

void AnimationController::registerAnimation(UINT Track, Animation* animation)
{	
	if (!_animationController && !animation) return;

	ID3DXKeyframedAnimationSet* anim = animation->_animationSet;
	_animationController->RegisterAnimationSet(anim);
	_animationController->SetTrackAnimationSet(Track, anim);

	D3DXTRACK_DESC Desc;
	Desc.Enable = true;
	Desc.Speed = 1;
	Desc.Weight = 1;
	Desc.Position = 0;
	Desc.Priority = D3DXPRIORITY_HIGH;

	_animationController->SetTrackDesc(Track, &Desc);
}

void  AnimationController::registerTransform(Transform* transform)
{
	if (!_animationController && !transform) return;
	LPD3DXMATRIX tempMatrix = &transform->_frame->_matrixContainer->Matrix;
	_animationController->RegisterAnimationOutput(transform->_name, tempMatrix, NULL, NULL, NULL);
}

void AnimationController::tempUpdate()
{
	static DWORD Time = GetTickCount();

	//_animationController->ResetTime();
	_animationController->AdvanceTime((GetTickCount() - Time) * 0.001f, 0);

	Time = GetTickCount();
}