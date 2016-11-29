#pragma once
#include "IResourceItem.h"
#include "stdafx.h"

// 1. operator overloading �ؾ��� (new, delete)
//   - ��� �������� ��. ResourceManager������ �ε�Ǵ� ��ü�� �ƴ�.
//   - SingletonPool ���.
// 2. animationController ������ ������ ���·� �̷������ ��.(Flyweight) ResourceManager�� ���� �����Ǿ����ų� ������Ʈ�� Get�Լ��� ������.
// 3. AnimatorController���� 

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
	void tempUpdate(); //�ӽ� Test��;
};

