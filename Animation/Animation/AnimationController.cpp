//#include "AnimationController.h"
//#include "Animation.h"
//#include "Frame.h"
//
//AnimationController::AnimationController()
//	: _animationController(NULL),
//	_animations(NULL)
//{
//	_animations = new std::list<Animation *>();
//	_frames = new std::list<Frame*>();
//}
//
//
//AnimationController::~AnimationController()
//{	
//	_frames->clear();
//	_animations->clear();
//	SAFE_DELETE(_frames);
//	SAFE_DELETE(_animations);
//	SAFE_RELEASE(_animationController);
//}
//
//void AnimationController::registerAnimation(Animation* animation)
//{	
//	if (!animation) return;
//
//	HRESULT hr = 0;
//	_animations->push_back(animation);
//}
//
//void AnimationController::registerFrame(Frame* frame)
//{
//	if (!_animationController && !frame) return;
//	
//	_frames->push_back(frame);
//}
//
//void AnimationController::create()
//{
//	DWORD numBone = _animations->front()->_numAnimation;
//	D3DXCreateAnimationController(numBone, numBone, numBone, numBone, &_animationController);
//
//	int numTrack = 0;
//	for (std::list<Animation*>::iterator it = _animations->begin(); it != _animations->end(); ++it)
//	{
//		
//		ID3DXKeyframedAnimationSet* anim = (*it)->_animationSet;
//		_animationController->RegisterAnimationSet(anim);
//		_animationController->SetTrackAnimationSet(numTrack, anim);
//
//		D3DXTRACK_DESC Desc;
//		Desc.Enable = true;
//		Desc.Speed = 1;
//		Desc.Weight = 1;
//		Desc.Position = 0;
//		Desc.Priority = D3DXPRIORITY_HIGH;
//
//		_animationController->SetTrackDesc(0, &Desc);
//
//		numTrack++;
//	}
//	
//	/*for (std::list<Frame*>::iterator it = _frames->begin(); it != _frames->end(); ++it)
//	{
//		D3DXMATRIX tempMatrix = (*it)->_matrixContainer->Matrix;
//		_animationController->RegisterAnimationOutput((*it)->getName(), &tempMatrix, NULL, NULL, NULL);
//	}*/
//}
//
//void AnimationController::tempUpdate()
//{
//	_animationController->ResetTime();
//	_animationController->AdvanceTime(160, 0);
//}