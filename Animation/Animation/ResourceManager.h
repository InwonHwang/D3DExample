#pragma once
#include "stdafx.h"

//class Animation;

class Animation;
class MeshRenderer;

class ResourceManager
{
private:
	std::list<D3DMATERIAL9*>        _materials;
	std::list<IDirect3DTexture9*>   _textures;
	std::list<Animation*>			_animations;
	DWORD i;

public:
	void parse(char* fileName);
	void parseChild(LPD3DXFILEDATA);
	
public:
	ResourceManager();
	~ResourceManager();

	MeshRenderer* _meshRenderer;
	void tempUpdate();
};

