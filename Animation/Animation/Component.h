#pragma once
//1. Component는 나중에 Scene Node가 될 GameObject 내에서만 생성이 가능 해야 함.

class Component abstract
{
public:	
	virtual ~Component();
	virtual void Update() = 0;
};