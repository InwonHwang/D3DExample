#pragma once
#include "Component.h"
#include "stdafx.h"

class Transform : public Component
{
private:
	std::list<Component *> _children;
	Component * _parent;
	LPD3DXFRAME _frame;
	
public:
	Transform();
	~Transform();
};

