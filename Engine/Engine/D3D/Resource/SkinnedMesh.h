#pragma once
#include "Mesh.h"

class SkinnedMesh : public Mesh
{
	typedef std::vector<int> BoneIndexVec;
public:
	SkinnedMesh(ResourceHandle handle, ResourcePoolImpl* pool);
	~SkinnedMesh();

	virtual void Destroy() override;

	virtual bool Create(IDirect3DDevice9& device, sp<FBXMESHDATA> fbxData) override;

	sp<std::vector<int>> GetBoneIndex() const;
private:
	sp<BoneIndexVec> _boneIndexVec;
};

inline sp<SkinnedMesh::BoneIndexVec> SkinnedMesh::GetBoneIndex() const
{
	return _boneIndexVec;
}

