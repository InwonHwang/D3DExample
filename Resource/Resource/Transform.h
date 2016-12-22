#pragma once
#include "IComponent.h"
#include "Frame.h"

class ResourceManager;
class AnimationController;
class Transform : public IComponent
{
	friend class ResourceManager;
	friend class AnimationController;
private:
	Frame* _frame;
	Transform *_parent;
	std::list<Transform *> _children;
	char*	_name;

public:
	Transform();
	~Transform();

	virtual void update() {}
};

