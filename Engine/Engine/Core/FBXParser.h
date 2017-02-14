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

	void Load(FBXData& fbxData);

private:
	void LoadRecursively(FbxNode& fbxNode, FBXData& fbxData);

	void ReadSkinInfo(FbxNode& fbxNode, FBXData& fbxData);

	// Mesh 파싱
	void ReadMesh(FbxNode& fbxNode, FBXData& fbxData);	
		
	void ReadPosition(FbxMesh& mesh, int ctrlPointIndex, Vector3& position);
	void ReadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector4& color);
	void ReadUV(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector2& uv);
	void ReadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector3& normal);
	void ReadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector3& tangent);

	

	//void ReadSkeleton(FbxNode& fbxNode);

private:
	FbxScene* _scene;
};

