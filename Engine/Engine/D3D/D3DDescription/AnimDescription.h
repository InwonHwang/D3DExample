#pragma once

#include "..\..\Core\Core.h"

typedef struct tagVectorKey	// �ִϸ��̼� Ű
{
	int frame;
	Vector3 value;
}VECTORKEY, *LPVECTORKEY;

typedef struct tagQuaternionKey	// �ִϸ��̼� Ű
{
	int frame;
	Quaternion value;
}QUATERNIONKEY, *LPQUATERNIONKEY;

typedef struct tagAnimKeySet
{
	tagAnimKeySet() {}
	~tagAnimKeySet()
	{
		std::vector<VECTORKEY> emptyData;
		scale.swap(std::vector<VECTORKEY>());
		rotation.swap(std::vector<QUATERNIONKEY>());
		position.swap(std::vector<VECTORKEY>());
	}

	std::vector<VECTORKEY> scale;
	std::vector<QUATERNIONKEY> rotation;
	std::vector<VECTORKEY> position;
	int length;
	int start;
	int end;

	String name;
}ANIMATIONKEYSET, *LPANIMATIONKEYSET;