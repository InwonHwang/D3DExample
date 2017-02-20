#pragma once

#include "..\..\Core\Core.h"
#include "Component.h"
#include "..\Resource\D3DResource.h"

class SkinnedMeshRenderer : public Component
{
	typedef std::vector<sp<FBXBONEDATA>> BoneDataVec;
	typedef std::vector<sp<SurfaceMaterial>> MaterialVec;
public:
	SkinnedMeshRenderer();
	~SkinnedMeshRenderer();

	virtual void Destroy() override;

	//virtual void Draw(IDirect3DDevice9& device);
	virtual void Draw(IDirect3DDevice9& device);
	
	// Test¿ë
	//void ApplyMatrix(IDirect3DDevice9& device);
	void ApplyMatrix(IDirect3DDevice9& device);
	void Test(IDirect3DDevice9& device, int frame);
	void SetBone(BoneDataVec b)
	{
		_boneDataVec = b;
	}

	void SetMesh(sp<SkinnedMesh> mesh);
	void SetMaterialCount(uint count);
	void SetMaterial(uint index, const sp<SurfaceMaterial> material);

private:
	sp<MaterialVec>	_materials;
	sp<SkinnedMesh>	_mesh;

	BoneDataVec _boneDataVec;
};

