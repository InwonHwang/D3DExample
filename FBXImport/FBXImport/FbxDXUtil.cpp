#include "FbxDXUtil.h"

D3DXQUATERNION FbxDxUtil::ToDXQuaternion(const FbxDouble4 &m, bool neg_w) {
	return D3DXQUATERNION((float)m[0], (float)m[1], (float)m[2], 1 * (float)m[3]);
}

D3DXVECTOR4 FbxDxUtil::ToDXVector4(const FbxDouble4 &m) {
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

D3DXVECTOR3 FbxDxUtil::ToDXVector3(const FbxDouble3 &m) {
	return D3DXVECTOR3((float)m[0], (float)m[1], (float)m[2]);
}


D3DXMATRIX FbxDxUtil::ToDXMatrix(const FbxAMatrix &mtx)
{
	D3DXMATRIX matS, matR, matT;
	
	auto s = ToDXVector3(mtx.GetS());
	auto r = ToDXQuaternion(mtx.GetQ(), false);
	auto t = ToDXVector3(mtx.GetT());
	
	D3DXMatrixScaling(&matS, s.x, s.y, s.z);
	D3DXMatrixRotationQuaternion(&matR, &r);
	D3DXMatrixTranslation(&matT, t.x, t.y, t.z);

	return matS * matR * matT;
}
