#pragma once
//1. IComponent�� ���߿� Scene Node�� �� GameObject �������� ������ ���� �ؾ� ��.

class IComponent abstract
{
public:	
	virtual ~IComponent() {}
	virtual void update() = 0;
};