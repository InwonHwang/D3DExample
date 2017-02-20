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

	// 소프트웨어 스키닝 테스트용
public:
	SKINNEDMESHVERTEX* pSkinnedMeshVertex;
	TEMPVERTEX* pVertexDraw;
	void*		pIndex;

private:
	sp<BoneIndexVec> _boneIndexVec;
};

inline sp<SkinnedMesh::BoneIndexVec> SkinnedMesh::GetBoneIndex() const
{
	return _boneIndexVec;
}

