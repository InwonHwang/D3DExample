#include "stdafx.h"

void DebugBox(HRESULT hr, LPCWSTR str)
{
	WCHAR szBuffer[50];
	swprintf_s(szBuffer, L"%i", hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);

}

void DebugBox(HRESULT hr, LPCSTR str)
{
	char szBuffer[50];
	sprintf_s(szBuffer, "%i", hr);


	MessageBoxA(GetActiveWindow(), szBuffer, str, MB_OK); 
}

void DebugError(HRESULT hr)
{

	MessageBox(GetActiveWindow(), DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK);
}

void Debug(float f, LPCSTR str)
{
	char szBuffer[50];
	sprintf_s(szBuffer, "%f", f);


	MessageBoxA(GetActiveWindow(), szBuffer, str, MB_OK);
}