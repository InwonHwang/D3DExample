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
	//	FbxDxUtil();	// 컴파일러가 디폴트 생성자를 못만들게 함.
	//	~FbxDxUtil();
};