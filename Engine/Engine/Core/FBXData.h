#pragma once
// FBX 파일의 파싱한 정보를 담아주는 클래스 정의

#include "..\ThirdParty\ThirdParty.h"
#include "Math\Math.h"
#include "DataStructure.h"
#include "DataTypes.h"

// 버텍스
typedef struct tagVertex
{
	tagVertex()
	{
		memset(&position, 0, sizeof(Vector3));
		memset(&color, 0, sizeof(Vector3));
		memset(&normal, 0, sizeof(Vector3));
		memset(&texCoord, 0, sizeof(Vector3));
		memset(&tangent, 0, sizeof(Vector3));
	}

	bool operator == (const tagVertex& rhs)
	{
		return (position == rhs.position) &
			(color == rhs.color) &
			(normal == rhs.normal) &
			(texCoord == rhs.texCoord) &
			(tangent == rhs.tangent);
	}

	Vector3 position;
	Vector4 color;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;

}VERTEX, *LPVERTEX;

// 스키닝 정보
typedef struct tagVertexBlendingInfo
{
	uint blendingIndex;
	float blendingWeight;

	tagVertexBlendingInfo() :
		blendingIndex(0),
		blendingWeight(0.0)
	{}	

	bool operator < (const tagVertexBlendingInfo& rhs)
	{
		return (blendingWeight > rhs.blendingWeight);
	}

	bool operator == (const tagVertexBlendingInfo& rhs)
	{		
		return (blendingIndex == rhs.blendingIndex) & Mathf::EqualWithEpsilon(blendingWeight, rhs.blendingWeight);
	}
}VERTEXBLENDINGINFO, *LPVERTEXBLENDINGINFO;

// 스키닝 정보 + 버텍스정보
typedef struct tagSkinnedVertex
{
	VERTEX vertex;
	std::vector<VERTEXBLENDINGINFO> vertexBlendingInfoVec;

	~tagSkinnedVertex()
	{
		vertexBlendingInfoVec.swap(std::vector<VERTEXBLENDINGINFO>());
	}

	void SortBlendingInfoByWeight()
	{
		std::sort(vertexBlendingInfoVec.begin(), vertexBlendingInfoVec.end());
	}
}SKINNEDVERTEX, LPSKINNEDVERTEX;

// 애니메이션 키 (Matrix 값)
typedef struct tagKeyframe
{
	FbxLongLong frame;
	FbxAMatrix globalTransform;
	String name;

	tagKeyframe()
	{
		frame = 0;
		globalTransform.SetIdentity();
	}
}KEYFRAME, *LPKEYFRAME;

typedef struct tagFBXData
{
	enum DataType
	{
		eTransform,
		eBone,
		eMesh		
	};

	int dataType;

}FBXDATA,* LPFBXDATA;

// 트랜스폼 값
typedef struct tagFBXTransformData : public FBXDATA
{
	String name;
	FbxAMatrix local;
	FbxAMatrix worldParent;
	int parentIndex;

	tagFBXTransformData()
	{
		local.SetIdentity();
		worldParent.SetIdentity();
	}

}FBXTRANSFORMDATA, *LPFBXTRANSFORMDATA;


// 뼈대 및 애니메이션 정보
typedef struct tagFBXBoneData : public tagFBXTransformData
{
	FbxAMatrix globalBindposeInverse;
	std::vector<sp<KEYFRAME>> animVec;
	FbxNode* pNode;
	int start;
	int end;

	tagFBXBoneData()		
		:
		pNode(nullptr),
		start(0),
		end(0)
	{		
		globalBindposeInverse.SetIdentity();
	}
	~tagFBXBoneData()
	{	
		animVec.swap(std::vector<sp<KEYFRAME>>());
		pNode = nullptr;
	}
}FBXBONEDATA, *LPFBXBONEDATA;

typedef struct tagFBXMeshData : public tagFBXTransformData
{
	std::vector<SKINNEDVERTEX> controlPointDataVec; //vertex Data
	std::vector<SKINNEDVERTEX> verticeDataVec; //vertex Data
	std::vector<uint>	indiceDataVec; //indeice Data
	bool isSkinned;

	~tagFBXMeshData()
	{
		controlPointDataVec.swap(std::vector<SKINNEDVERTEX>());
		verticeDataVec.swap(std::vector<SKINNEDVERTEX>());
		indiceDataVec.swap(std::vector<uint>());
	}

}FBXMESHDATA, *LPFBXMESHDATA;

typedef struct tagFBXDataSet
{
	std::vector<int> lengthAnim;
	std::vector<sp<FBXDATA>> fbxDataVec;

	~tagFBXDataSet()
	{
		fbxDataVec.swap(std::vector<sp<FBXDATA>>());
	}
}FBXDATASET, *LPFBXDATASET;