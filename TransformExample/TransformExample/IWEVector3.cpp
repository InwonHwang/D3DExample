#include "IWEVector3.h"

IWEVector3::IWEVector3(const IWEVector3& v)
	: D3DXVECTOR3(v)
{
}
IWEVector3::IWEVector3(FLOAT x, FLOAT y, FLOAT z)
	: D3DXVECTOR3(x, y, z)
{
}