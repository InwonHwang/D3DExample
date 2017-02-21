#include "FbxDXUtil.h"



FbxDXUtil::FbxDXUtil()
{
}


FbxDXUtil::~FbxDXUtil()
{
}

Quaternion FbxDXUtil::ToQuaternion(const FbxQuaternion &m) {
	return Quaternion((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

Vector4 FbxDXUtil::ToVector4(const FbxDouble4 &m)
{
	return Vector4((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

Vector3 FbxDXUtil::ToVector3(const FbxDouble3 &m) {
	return Vector3((float)m[0], (float)m[1], (float)m[2]);
}

D3DXMATRIX FbxDXUtil::ToDXMatrix(const FbxAMatrix &mtx)
{
	D3DXMATRIX ret;

	for (int i = 0; i < 4; ++i)
	{
		ret.m[i][0] = float(mtx.Get(i, 0));
		ret.m[i][1] = float(mtx.Get(i, 1));
		ret.m[i][2] = float(mtx.Get(i, 2));
		ret.m[i][3] = float(mtx.Get(i, 3));
	}

	return ret;
}
