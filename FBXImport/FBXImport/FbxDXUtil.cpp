#include "FbxDXUtil.h"



D3DXVECTOR4 FbxDxUtil::ToDX(const FbxDouble4 &m, bool neg_w) {
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (neg_w ? -1 : 1) * (float)m[3]);
}

D3DXVECTOR4 FbxDxUtil::ToDX(const FbxDouble4 &m) {
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

D3DXVECTOR3 FbxDxUtil::ToDX(const FbxDouble3 &m) {
	return D3DXVECTOR3((float)m[0], (float)m[1], (float)m[2]);
}


D3DXMATRIX FbxDxUtil::ToDX(const FbxAMatrix &mtx)
{
	D3DXMATRIX mtx_s, mtx_r, mtx_t;

	auto s = ToDX(mtx.GetS());
	auto q = ToDX(mtx.GetQ(), false);
	auto t = ToDX(mtx.GetT());

	return
		*D3DXMatrixScaling(&mtx_s, (float)s[0], (float)s[1], (float)s[2]) *
		*D3DXMatrixRotationQuaternion(&mtx_r, &D3DXQUATERNION((float)q[0], (float)q[1], (float)q[2], (float)q[3])) *
		*D3DXMatrixTranslation(&mtx_t, (float)t[0], (float)t[1], (float)t[2]);
}
