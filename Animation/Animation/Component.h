#pragma once
//1. Component�� ���߿� Scene Node�� �� GameObject �������� ������ ���� �ؾ� ��.

class Component abstract
{
public:	
	virtual ~Component();
	virtual void Update() = 0;
};