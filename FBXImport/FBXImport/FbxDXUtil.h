#pragma once
#include "Core.h"

class FbxDxUtil abstract
{
public:

	static D3DXQUATERNION ToDXQuaternion(const FbxQuaternion &m);

	static D3DXVECTOR4 ToDXVector4(const FbxDouble4 &m);

	static D3DXVECTOR3 ToDXVector3(const FbxDouble3 &m);

	static D3DXMATRIX ToDXMatrix(const FbxAMatrix &mtx);

	

	//private:
	//	FbxDxUtil();	// �����Ϸ��� ����Ʈ �����ڸ� ������� ��.
	//	~FbxDxUtil();
};