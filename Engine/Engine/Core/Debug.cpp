#include "Debug.h"

Debug::Debug()
{
}


Debug::~Debug()
{
}

void Debug::MsgBox(HRESULT hr, LPCTSTR str)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
}

String Debug::IntegerToString(int i)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), i);

	return String(szBuffer);
}

String Debug::FloatToString(float f)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%f"), f);

	return String(szBuffer);
}


