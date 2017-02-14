#pragma once
// FBX 파일의 파싱한 정보를 담아주는 클래스 정의

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

	D3DXMATRIX	_matLocal;				// 로컬 행렬
	D3DXMATRIX	_matWorldParent;		// 부모의 월드 행렬

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

	D3DXMATRIX	_matLocal;				// 로컬 행렬
	D3DXMATRIX	_matWorldParent;		// 부모의 월드 행렬

	std::vector<Vector3> _positionVec;	// 위치
	std::vector<Vector4> _colorVec;		// 색
	std::vector<Vector3> _normalVec;	// 노멀
	std::vector<Vector3> _binormalVec;	// 바이노멀
	std::vector<Vector3> _tangentVec;	// 탄젠트
	std::vector<Vector2> _texCroodVec;	// 텍스쳐 uv
	std::vector<SKININFO> _skinInfo;	// 정점 무게값

	std::vector<INDEX>	 _indiceVec;	// 인덱스

	bool _isSkinned;
};

class FBXTrackData	// 애니메이션 트랙
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