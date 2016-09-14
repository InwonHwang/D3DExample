#include "IWEQuaternion.h"


IWEQuaternion::IWEQuaternion(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
	: D3DXQUATERNION(x, y, z, w)
{
}

IWEQuaternion::IWEQuaternion(const IWEQuaternion& q)
	: D3DXQUATERNION(q)
{
}

//static
float IWEQuaternion::Angle(const IWEQuaternion& q1, const IWEQuaternion& q2)
{
	D3DXQUATERNION inv, res;
	D3DXQuaternionInverse(&inv, &q1);
	res = q2;
	return IWEMath::Acos(res.w);
}

float IWEQuaternion::Dot(const IWEQuaternion& q1, const IWEQuaternion& q2)
{
	return D3DXQuaternionDot(&q1, &q2);
}

//IWEQuaternion IWEQuaternion::AngleAxis(float d, const IWEVector3& v)
//{
//	IWEQuaternion ret;
//	D3DXQuaternionRotationAxis(&ret, &v, d);
//	return ret;
//}

IWEQuaternion IWEQuaternion::Inverse(const IWEQuaternion& q)
{
	IWEQuaternion ret;
	D3DXQuaternionInverse(&ret, &q);
	return ret;
}

IWEQuaternion IWEQuaternion::SLerp(const IWEQuaternion& q1, const IWEQuaternion& q2, float m)
{
	IWEQuaternion ret;
	D3DXQuaternionSlerp(&ret, &q1, &q2, m);
	return ret;
}

IWEQuaternion IWEQuaternion::Euler(float x, float y, float z)
{
	IWEQuaternion ret;
	D3DXQuaternionRotationYawPitchRoll(&ret, y * IWEMath::DegToRad(), x * IWEMath::DegToRad(), z * IWEMath::DegToRad());
	return ret;
}