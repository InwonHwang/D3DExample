#pragma once
#include "stdafx.h"

class AnimationController;
class Animation;
class MeshRenderer;
class Transform;

class ResourceManager
{
private:
	std::list<D3DMATERIAL9*>        _materials;
	std::list<IDirect3DTexture9*>   _textures;
	std::list<Animation*>			_animations;

public:
	void parse(char* fileName);
	void parseChild(LPD3DXFILEDATA, void*);
	
public:
	ResourceManager();
	~ResourceManager();

	MeshRenderer*	_meshRenderer;
	Transform*		_topTransform;
	AnimationController* _animationController;
	D3DXMATRIX _tempMatrix;

	void tempUpdate(); //test��
	void tempUpdateFrame(Transform *); //test��	
	void tempClearTransform(Transform *); // test��
	void tempLinkFrame(Transform *); // test��
	void tempRegisterFrame(AnimationController*, Transform *); // test��

};

