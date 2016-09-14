#pragma once
#include "stdafx.h"


class IWEQuaternion : public D3DXQUATERNION
{
public:
	using D3DXQUATERNION::operator=;

	IWEQuaternion() {}
	IWEQuaternion(const IWEQuaternion&);
	IWEQuaternion(FLOAT x, FLOAT y, FLOAT z, FLOAT w);

	static float Angle(const IWEQuaternion&, const IWEQuaternion&);
	static float Dot(const IWEQuaternion&, const IWEQuaternion&);
	static IWEQuaternion Euler(float x, float y, float z);
	//static IWEQuaternion AngleAxis(float, const IWEVector3&);
	static IWEQuaternion Inverse(const IWEQuaternion&);
	static IWEQuaternion SLerp(const IWEQuaternion&, const IWEQuaternion&, float);
};

