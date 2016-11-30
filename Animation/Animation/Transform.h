#pragma once
#include "Component.h"
#include "Frame.h"

class ResourceManager;

class Transform : public Component
{
	friend class ResourceManager;
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

