#pragma once
#include "stdafx.h"

// 1. operator overloading �ؾ��� (new, delete)
//   - ��� �������� ��. ResourceManager������ �ε�Ǵ� ��ü�� �ƴ�.
//   - SingletonPool ���.
// 2. animationController ������ ������ ���·� �̷������ ��.(Flyweight) ResourceManager�� ���� �����Ǿ����ų� ������Ʈ�� Get�Լ��� ������.
//   - �ִϸ��̼��� ���������
// 3. AnimatorController���� 

class Transform;
class Animation;
class Frame;

class AnimationController
{
private:	
	ID3DXAnimationController* _animationController;	
	//std::list<Animation *>* _animations;
	//std::list<Transform *>* _trasnforms;
	//std::list<Frame *>* _frames;

public:
	AnimationController(UINT maxNumMatrices, UINT maxNumAnimations, UINT maxNumTracks, UINT maxNumEvents);
	~AnimationController();

	void registerAnimation(UINT, Animation*);
	void registerTransform(Transform*);
	void tempRegisterTransform(char* name, D3DXMATRIX* matrix)
	{
		_animationController->RegisterAnimationOutput(name, matrix, 0, 0, 0);
	}
	void tempUpdate(); //�ӽ� Test��;
};

