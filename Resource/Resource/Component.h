#pragma once

class Component abstract
{
public:	
	virtual ~Component();
	virtual void Update() = 0;
};

