#pragma once
#include "stdafx.h"


class IWEVector3 : public D3DXVECTOR3
{
public:
	using D3DXVECTOR3::operator=;

	IWEVector3() {};
	IWEVector3(const IWEVector3&);
	IWEVector3(FLOAT x, FLOAT y, FLOAT z);
};


