#pragma once

template<typename T>
class Component abstract
{
public:
	virtual ~Component() {}

	virtual void init() = 0;
	virtual void update() = 0;
};
