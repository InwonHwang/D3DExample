#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxfile.h>
#include <DxErr.h>
#include <tchar.h>
#include <string>
#include <list>
#include <vector>

#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; }

namespace std { typedef basic_string<TCHAR> tstring; }

void DebugError(HRESULT hr);
void DebugBox(HRESULT hr, LPCTSTR str);
float Lerp(float a, float b, float t);
DWORD FtoDw(float f);
float GetRandomFloat(float lowBound, float highBound);
void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);

struct BoundingBox
{
	BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};

struct BoundingSphere
{
	BoundingSphere();

	D3DXVECTOR3 _center;
	float       _radius;
};