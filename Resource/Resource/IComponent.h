#pragma once
//1. IComponent는 나중에 Scene Node가 될 GameObject 내에서만 생성이 가능 해야 함.

class IComponent abstract
{
public:	
	virtual ~IComponent() {}
	virtual void update() = 0;
};