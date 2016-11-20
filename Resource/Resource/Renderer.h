#pragma once
#include "Component.h"

class Renderer : public Component
{


public:
	Renderer();
	~Renderer();

	virtual void Update() override;
};

