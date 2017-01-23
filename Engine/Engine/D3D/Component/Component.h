#pragma once

class Component
{
public:
	Component() {}
	~Component() {}

	virtual void Destroy() = 0;

private:	
	bool _enabled;
};