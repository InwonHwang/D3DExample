#include "Transform.h"



Transform::Transform()
	: _frame(NULL)
{
	_frame = new Frame();
}


Transform::~Transform()
{
	_parent = NULL;
	_children.clear();	
	SAFE_DELETE_ARRAY(_name);
	SAFE_DELETE(_frame);
}
