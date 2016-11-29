#pragma once
#include "IResourceItem.h"
#include "stdafx.h"

// 1. operator overloading 해야함 (new, delete)
//   - 몇개가 생성될지 모름. ResourceManager에의해 로드되는 객체가 아님.
//   - SingletonPool 사용.
// 2. animationController 공유는 포인터 형태로 이루워져야 함.(Flyweight) ResourceManager에 의해 공유되어지거나 컴포넌트의 Get함수로 공유됨.
// 3. AnimatorController에서 

class Animation;
class Frame;

class AnimationController
{
private:	
	ID3DXAnimationController* _animationController;	
	std::list<Animation *>* _animations;
	std::list<Frame *>* _frames;

public:
	AnimationController();
	~AnimationController();

	void registerAnimation(Animation*);
	void registerFrame(Frame*);
	void create();
	void tempUpdate(); //임시 Test용;
};

