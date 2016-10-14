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