#pragma once
#include "..\ThirdParty\ThirdParty.h"
#include "Math\Math.h"

class FbxDXUtil
{
public:
	FbxDXUtil();
	~FbxDXUtil();

	static Quaternion ToQuaternion(const FbxQuaternion &m);
	static Vector4 ToVector4(const FbxDouble4 &m);
	static Vector3 ToVector3(const FbxDouble3 &m);
	static D3DXMATRIX ToDXMatrix(const FbxAMatrix &mtx);
};

