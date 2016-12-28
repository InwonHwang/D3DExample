#include "Core.h"

void DebugBox(HRESULT hr, LPCTSTR str)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
}

std::tstring floatToString(float f)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%f"), f);

	return std::tstring(szBuffer);
}
