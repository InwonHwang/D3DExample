#include <Windows.h>
#include "Core\Core.h"

void DebugBox(HRESULT hr, LPCTSTR str)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
}

void testBitFlags()
{
	flags32 flag2;

	flag2.set(30);

	String str = TEXT("HELLO");
	DebugBox(0, OUTPUT_TEXT(str));	
}


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		_T("D3D Tutorial"), NULL
	};
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(_T("D3D Tutorial"), _T("D3D Tutorial 01: CreateDevice"),
		WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	SetActiveWindow(hWnd);	

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	testBitFlags();

	while (msg.message != WM_QUIT)
	{			

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	UnregisterClass(_T("D3D Tutorial"), wc.hInstance);
	return 0;
}




