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

float Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

bool IsInRect(RECT* rc, POINT pt)
{
	if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
		(rc->bottom <= pt.y) && (pt.y <= rc->top))
		return true;

	return false;
}
