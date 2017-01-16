#include "FbxDXUtil.h"

D3DXQUATERNION FbxDxUtil::ToDXQuaternion(const FbxQuaternion &m) {
	return D3DXQUATERNION((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

D3DXVECTOR4 FbxDxUtil::ToDXVector4(const FbxDouble4 &m) {
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

D3DXVECTOR3 FbxDxUtil::ToDXVector3(const FbxDouble3 &m) {
	return D3DXVECTOR3((float)m[0], (float)m[1], (float)m[2]);
}


D3DXMATRIX FbxDxUtil::ToDXMatrix(const FbxAMatrix &mtx)
{

	D3DXMATRIX ret;

	for (int i = 0; i < 4; ++i)
	{
		ret.m[i][0] = static_cast<float>(mtx.Get(i, 0));
		ret.m[i][1] = static_cast<float>(mtx.Get(i, 1));
		ret.m[i][2] = static_cast<float>(mtx.Get(i, 2));
		ret.m[i][3] = static_cast<float>(mtx.Get(i, 3));
	}

	return ret;
}
