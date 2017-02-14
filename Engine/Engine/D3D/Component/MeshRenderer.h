#pragma once

#include "..\..\Core\Core.h"
#include "Component.h"
#include "..\Resource\D3DResource.h"

class MeshRenderer : public Component
{
	typedef std::vector<sp<SurfaceMaterial>> MaterialVec;
public:
	MeshRenderer();
	~MeshRenderer();

	virtual void Destroy() override;

	virtual void Draw(IDirect3DDevice9& device);

	void SetMesh(sp<Mesh> mesh);
	void SetMaterialCount(uint count);
	void SetMaterial(uint index, const sp<SurfaceMaterial> material);

private:
	sp<MaterialVec>	_materials;
	sp<Mesh>		_mesh;	
};

