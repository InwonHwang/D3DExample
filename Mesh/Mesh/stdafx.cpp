#include "stdafx.h"

void DebugError(HRESULT hr)
{
	MessageBox(GetActiveWindow(), DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK);
}

void DebugBox(HRESULT hr, LPCTSTR str)
{
	TCHAR szBuffer[50];
	_stprintf(szBuffer, _T("%i"), hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
}