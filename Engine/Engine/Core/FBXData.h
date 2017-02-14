#pragma once
// FBX ������ �Ľ��� ������ ����ִ� Ŭ���� ����

#include "..\ThirdParty\ThirdParty.h"
#include "D3DDescription\D3DDescription.h"
#include "Math\Math.h"
#include "DataStructure.h"
#include "DataTypes.h"

class FBXTransformData
{
public:
	FBXTransformData() {}
	~FBXTransformData() {}

	D3DXMATRIX	_matLocal;				// ���� ���
	D3DXMATRIX	_matWorldParent;		// �θ��� ���� ���

	bool _isBone;
};

class FBXMeshData
{
public:
	FBXMeshData() {}
	~FBXMeshData()
	{	
		_positionVec.clear();
		_colorVec.clear();
		_normalVec.clear();
		_binormalVec.clear();
		_tangentVec.clear();
		_texCroodVec.clear();
		_skinInfo.clear();
		_indiceVec.clear();

		_positionVec.resize(0);
		_colorVec.resize(0);
		_normalVec.resize(0);
		_binormalVec.resize(0);
		_tangentVec.resize(0);
		_texCroodVec.resize(0);
		_skinInfo.resize(0);
		_indiceVec.resize(0);
	}

	D3DXMATRIX	_matLocal;				// ���� ���
	D3DXMATRIX	_matWorldParent;		// �θ��� ���� ���

	std::vector<Vector3> _positionVec;	// ��ġ
	std::vector<Vector4> _colorVec;		// ��
	std::vector<Vector3> _normalVec;	// ���
	std::vector<Vector3> _binormalVec;	// ���̳��
	std::vector<Vector3> _tangentVec;	// ź��Ʈ
	std::vector<Vector2> _texCroodVec;	// �ؽ��� uv
	std::vector<SKININFO> _skinInfo;	// ���� ���԰�

	std::vector<INDEX>	 _indiceVec;	// �ε���

	bool _isSkinned;
};

class FBXTrackData	// �ִϸ��̼� Ʈ��
{
	std::vector<AnimKey> scale;	
	std::vector<AnimKey> rotation;
	std::vector<AnimKey> position;
	float start;
	float end;
	
	~FBXTrackData()
	{
		std::vector<AnimKey> emptyData;
		scale.swap(emptyData);
		rotation.swap(emptyData);
		position.swap(emptyData);
	}
};

class FBXMeshDataMgr
{
public:
	~FBXMeshDataMgr()
	{
		for (auto data : _fbxMeshDataVec)
			data.reset();

		_fbxMeshDataVec.clear();
	}

	std::vector<sp<FBXMeshData>> _fbxMeshDataVec;
};

class FBXAnimationData
{
public:
	~FBXAnimationData()
	{
		for (auto data : _fbxTrackDataVec)
			data.reset();

		_fbxTrackDataVec.clear();
	}

	std::vector<sp<FBXTrackData>> _fbxTrackDataVec;
};

class FBXAnimationDataMgr
{
public:
	~FBXAnimationDataMgr()
	{
		for (auto data : _fbxAnimationDataVec)
			data.reset();

		_fbxAnimationDataVec.clear();
	}

	std::vector<sp<FBXAnimationData>> _fbxAnimationDataVec;
};

class FBXData
{
public:
	FBXMeshDataMgr _fbxMeshDataMgr;
	FBXAnimationDataMgr _fbxAnimationDataMgr;
};