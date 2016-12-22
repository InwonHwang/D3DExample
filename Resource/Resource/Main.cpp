
#include "stdafx.h"
#include "Device.h"
#include "XFile.h"
#include "ResourceManager.h"

ResourceManager* rm;

void Init()
{
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	D3DVECTOR vecDirection = { 0.0f, 0.0f, 0.0f };
	light.Direction = vecDirection;
	D3DDevice->SetLight(0, &light);
	D3DDevice->LightEnable(0, TRUE);
}

void Render()
{
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(1000.0f, 100.0f, 0.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	D3DDevice->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		(FLOAT)800 / (FLOAT)600,    // aspect ratio
		1.0f,    // the near view-plane
		2000.0f);    // the far view-plane
	D3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	//static float index = 0.0f; index += 0.03f;    // an ever-increasing float value
	D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
	//D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
	D3DXMatrixRotationY(&matRotateY, 0);    // the rotation matrix
	D3DDevice->SetTransform(D3DTS_WORLD, &matRotateY);    // set the world transform

	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (SUCCEEDED(D3DDevice->BeginScene()))
	{		
		rm->tempUpdate();
		D3DDevice->EndScene();
	}

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
		SAFE_DELETE(rm);
		D3D->Release();
		XFILE->Release();
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

	if (D3D->Init() && XFILE->Init())
	{
		rm = new ResourceManager();
		rm->parse("tiny.x");		

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			DWORD starting_point = GetTickCount();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			
			Render();

			while ((GetTickCount() - starting_point) < 25);
		}
	}

	

	UnregisterClass(_T("D3D Tutorial"), wc.hInstance);
	return 0;
}



