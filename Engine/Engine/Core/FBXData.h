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

	bool operator==(const tagVertex& rhs) const
	{
		bool flag;

		flag = this->position == rhs.position;
		flag &= this->color == rhs.color;
		flag &= this->normal == rhs.normal;
		flag &= this->texCoord == rhs.texCoord;
		flag &= this->tangent == rhs.tangent;

		return flag;
	}
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
		return (blendingWeight < rhs.blendingWeight);
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

	bool operator == (const tagSkinnedVertex& rhs) const
	{
		bool flag = true;

		if (!(vertexBlendingInfoVec.empty() && rhs.vertexBlendingInfoVec.empty()))
		{
			for (uint i = 0; i < 4; ++i)
			{
				if (vertexBlendingInfoVec[i].blendingIndex != rhs.vertexBlendingInfoVec[i].blendingIndex ||
					abs(vertexBlendingInfoVec[i].blendingWeight - rhs.vertexBlendingInfoVec[i].blendingWeight) > 0.001)
				{
					flag = false;
					break;
				}
			}
		}

		flag &= vertex == rhs.vertex;

		return flag;
	}
}SKINNEDVERTEX, LPSKINNEDVERTEX;

// �ִϸ��̼� Ű (Matrix ��)
typedef struct tagKeyframe
{
	FbxLongLong frameCount;
	FbxAMatrix globalTransform;
	tagKeyframe* pNext;

	tagKeyframe()
	{
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
	//FbxNode* pNode;
	int start;
	int end;

	tagFBXBoneData()
		: pAnimation(nullptr),
		//pNode(nullptr),
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