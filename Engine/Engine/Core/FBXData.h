#pragma once
// FBX ������ �Ľ��� ������ ����ִ� Ŭ���� ����

#include "..\ThirdParty\ThirdParty.h"
#include "Math\Math.h"
#include "DataStructure.h"
#include "DataTypes.h"

// ���ؽ�
typedef struct tagVertex
{
	Vector3 position;
	Vector4 color;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;

}VERTEX, *LPVERTEX;

// ��Ű�� ����
typedef struct tagVertexBlendingInfo
{
	uint blendingIndex;
	double blendingWeight;

	tagVertexBlendingInfo() :
		blendingIndex(0),
		blendingWeight(0.0)
	{}

	bool operator < (const tagVertexBlendingInfo& rhs)
	{
		return (blendingWeight > rhs.blendingWeight);
	}
}VERTEXBLENDINGINFO, *LPVERTEXBLENDINGINFO;

// ��Ű�� ���� + ���ؽ�����
typedef struct tagSkinnedVertex
{
	VERTEX vertex;
	std::vector<VERTEXBLENDINGINFO> vertexBlendingInfoVec;

	void SortBlendingInfoByWeight()
	{
		std::sort(vertexBlendingInfoVec.begin(), vertexBlendingInfoVec.end());
	}
}SKINNEDVERTEX, LPSKINNEDVERTEX;

// �ִϸ��̼� Ű (Matrix ��)
typedef struct tagKeyframe
{
	FbxLongLong frame;
	FbxAMatrix globalTransform;
	tagKeyframe* pNext;

	tagKeyframe()
	{
		frame = 0;
		pNext = nullptr;
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

// Ʈ������ ��
typedef struct tagFBXTransformData : public FBXDATA
{
	String name;
	FbxAMatrix local;
	FbxAMatrix world;
	int parentIndex;

	tagFBXTransformData()
	{
		local.SetIdentity();
		world.SetIdentity();
	}

}FBXTRANSFORMDATA, *LPFBXTRANSFORMDATA;


// ���� �� �ִϸ��̼� ����
typedef struct tagFBXBoneData : public tagFBXTransformData
{
	FbxAMatrix globalBindposeInverse;
	KEYFRAME* pAnimation;
	int start;
	int end;

	tagFBXBoneData()
		: pAnimation(nullptr),
		start(0),
		end(0)
	{		
		globalBindposeInverse.SetIdentity();
	}
	~tagFBXBoneData() {
		while (pAnimation) {
			KEYFRAME* temp = pAnimation->pNext;
			delete pAnimation;
			pAnimation = temp;
		}
	}
}FBXBONEDATA, *LPFBXBONEDATA;

typedef struct tagFBXMeshData : public tagFBXTransformData
{
	std::vector<SKINNEDVERTEX> verticeDataVec; //vertex Data
	std::vector<uint>	indiceDataVec; //indeice Data
	bool isSkinned;

}FBXMESHDATA, *LPFBXMESHDATA;

typedef struct tagFBXDataSet
{
	std::vector<sp<FBXDATA>> fbxDataVec;
}FBXDATASET, *LPFBXDATASET;