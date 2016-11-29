#pragma once
#include "Renderer.h"
#include "stdafx.h"

class Mesh;
class ResourceManager;

class MeshRenderer : public Renderer
{
	friend class ResourceManager;
protected:
	Mesh* _mesh;
	std::list<D3DMATERIAL9 *> _materials;
	std::list<IDirect3DTexture9 *> _textures;
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void update() override;
};

