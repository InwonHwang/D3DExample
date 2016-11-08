#include "stdafx.h"

void DebugError(HRESULT hr)
{
	MessageBox(GetActiveWindow(), DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK);
}

void DebugBox(HRESULT hr, LPCTSTR str)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
}

float Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void GetRandomVector(
	D3DXVECTOR3* out,
	D3DXVECTOR3* min,
	D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

BoundingBox::BoundingBox()
{
	_min.x = INFINITY;
	_min.y = INFINITY;
	_min.z = INFINITY;

	_max.x = -INFINITY;
	_max.y = -INFINITY;
	_max.z = -INFINITY;
}

bool BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BoundingSphere::BoundingSphere()
{
	_radius = 0.0f;
}