#pragma once
#include "stdafx.h"

class Transform;
class Mesh;

class GameObject
{

public:
	Transform * transform;
	Mesh * mesh;

	void update();


public:
	GameObject();
	~GameObject();
};

