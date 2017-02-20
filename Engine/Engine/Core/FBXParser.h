#pragma once
#include "..\ThirdParty\ThirdParty.h"
#include "DataTypes.h"
#include "Debug.h"
#include "FBXData.h"
#include "FbxDXUtil.h"
#include "Singleton.h"

// 오직 1개만 존재 쓰레드 사용시 동기화 필요
class FBXMgr : public Singleton<FBXMgr>
{
	friend class Singleton<FBXMgr>;
public:	
	~FBXMgr() {}

	virtual bool Init() override;
	virtual void Release() override;

	FbxManager* Get() const;

private:
	FBXMgr() {}

private:
	FbxManager* _fbxManager;
};

inline FbxManager* FBXMgr::Get() const
{
	return _fbxManager;
}


class FBXParser
{
public:
	FBXParser() {}
	~FBXParser() {}

	bool Init(FbxManager& fbxManager, const String& fileName);
	void Release();

	void Load(FBXDATASET& fbxData);

private:
	void LoadRecursively(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex);

	// Transform Data 파싱
	void ReadTransform(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex);	

	// Bone Data 파싱
	void ReadBone(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex);
	
	void ReadMatrix(FbxNode& fbxNode, FbxAMatrix& local, FbxAMatrix& worldParent);

	void ReadSkinInfo(FbxNode& fbxNode, FBXDATASET& fbxData, sp<FBXMESHDATA> meshData);

	void ReadAnimation(FbxNode& fbxNode, std::vector<sp<FBXBONEDATA>>& fbxData);

	// Mesh 파싱
	void ReadMesh(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex);

	void ReadPositionControlPoint(FbxMesh& fbxMesh, int ctrlPointCount, Vector3& position);
	void ReadlPositionTriangle(FbxMesh& fbxMesh, int ctrlPointIndex, Vector3& position);
	void ReadColor(FbxMesh& fbxMesh, int ctrlPointIndex, int vertexCount, Vector4& color);
	void ReadUV(FbxMesh& fbxMesh, int ctrlPointIndex, int vertexCount, Vector2& uv);
	void ReadNormal(FbxMesh& fbxMesh, int ctrlPointIndex, int vertexCount, Vector3& normal);
	void ReadTangent(FbxMesh& fbxMesh, int ctrlPointIndex, int vertexCount, Vector3& tangent);
	void Optimize(sp<FBXMESHDATA> meshData);

	void GetGeometryMatrix(FbxNode& fbxNode, FbxAMatrix& matrix);

private:
	FbxScene* _scene;
	bool _existAnimation;
};

