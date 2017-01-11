#pragma once
#include "IMemento.h"
#include "Core.h"

class AnimationCurve;

class Animation : public IMemento
{
public:
	Animation();
	~Animation();

	virtual void Load(FbxNode& fbxNode, void* pData) override;
	virtual void Save(FbxNode& fbxNode) override;

	//void AddAnimationCurve() = 0;

private:
	std::vector<AnimationCurve *> _animCurve;
};

