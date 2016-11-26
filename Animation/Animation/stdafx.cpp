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

std::tstring IntegerToString(DWORD i)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), i);
	
	return std::tstring(szBuffer);
}

std::tstring floatToString(float f)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%f"), f);

	return std::tstring(szBuffer);
}