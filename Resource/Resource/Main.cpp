//-----------------------------------------------------------------------------
// File: CreateDevice.cpp
//
// Desc: This is the first tutorial for using Direct3D. In this tutorial, all
//       we are doing is creating a Direct3D device and using it to clear the
//       window.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "Device.h"
#include "Font.h"
#include "Timer.h"
#include "Mesh.h"

Font *font;
Timer timer;

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// Clear the backbuffer to a blue color

	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(D3DDevice->BeginScene()))
	{
		font->draw();

		D3DDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	D3DDevice->Present(NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler 
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		font->release();
		delete font;
		D3D->Release();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 01: CreateDevice",
		WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	Mesh *temp = new Mesh();

	delete temp;

	SetActiveWindow(hWnd);
	timer.SetUp();
	timer.start();

	if (D3D->Init())
	{
		font = new Font();

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		
		static float lastTime = timer.getCurrentTime();

		while (GetMessage(&msg, NULL, 0, 0))
		{
			float curTime = timer.getCurrentTime();
			float timeDelta = (curTime - lastTime) * 0.001f;

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				std::tstring temp = IntegerToString(1/timeDelta);
				font->setString(temp.c_str());
				Render();

				lastTime = curTime;
			}


			while ((timer.getCurrentTime() - curTime) < 30);
		}
	}



	UnregisterClass(L"D3D Tutorial", wc.hInstance);
	return 0;
}



