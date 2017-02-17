#pragma once

#include "..\..\Core\Core.h"

typedef struct tagAnimationKey	// 애니메이션 키
{
	int frame;
	Vector3 value;
}ANIMATIONKEY, *LPANIMATIONKEY;

typedef struct tagAnimKeySet
{
	tagAnimKeySet() {}
	~tagAnimKeySet()
	{
		std::vector<ANIMATIONKEY> emptyData;
		scale.swap(emptyData);
		rotation.swap(emptyData);
		position.swap(emptyData);
	}

	std::vector<ANIMATIONKEY> scale;
	std::vector<ANIMATIONKEY> rotation;
	std::vector<ANIMATIONKEY> position;
	int length;
	int start;
	int end;

	String name;
}ANIMATIONKEYSET, *LPANIMATIONKEYSET;