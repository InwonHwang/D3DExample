#pragma once
#include "stdafx.h"

//class Animation;

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

	void tempUpdateFrame(Transform *Top);
	void tempUpdate(); //test¿ë
	void tempClearTransform(Transform *Top); // test¿ë
	void tempResgisterFrame(Transform *Top); // test¿ë

};

