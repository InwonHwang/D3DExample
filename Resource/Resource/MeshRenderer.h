#pragma once
#include "Renderer.h"

class Mesh;
class Material;

class MeshRenderer : public Renderer
{
private:
	Mesh* _mesh;
	Material* _material;

public:
	MeshRenderer();
	~MeshRenderer();

	virtual void Update() override;
	virtual void SetMesh(Mesh* mesh);
	virtual void SetSubMesh(Material* material);
};

