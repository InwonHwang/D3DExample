
#include "stdafx.h"
#include "Device.h"
#include "Font.h"
#include "Timer.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Pool.h"

Font *font;
Mesh *mesh;
Material *material;
MeshRenderer *meshRenderer;
Timer timer;

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
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 10.0f, 40.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),
		800 / 600,
		1.0f,
		100.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection);

	float index = 1.5f;
	D3DXMATRIX matRotateY;
	D3DXMatrixRotationY(&matRotateY, index);
	D3DDevice->SetTransform(D3DTS_WORLD, &(matRotateY));

	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(D3DDevice->BeginScene()))
	{
		font->draw();
		meshRenderer->Update();
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
		SAFE_DELETE(font);
		SAFE_DELETE(mesh);
		SAFE_DELETE(meshRenderer);
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
		_T("D3D Tutorial"), NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(_T("D3D Tutorial"), _T("D3D Tutorial 01: CreateDevice"),
		WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	SetActiveWindow(hWnd);
	timer.SetUp();
	timer.start();

	if (D3D->Init())
	{
		font = new Font();
		mesh = new Mesh();
		material = new Material();
		meshRenderer = new MeshRenderer();		

		mesh->Load(_T("bigship1.x"));
		material->set();
		meshRenderer->SetMesh(mesh);
		meshRenderer->SetSubMesh(material);

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		
		static UINT32 lastTime = timer.getCurrentTime();

		while (GetMessage(&msg, NULL, 0, 0))
		{
			UINT32 curTime = timer.getCurrentTime();
			float timeDelta = (curTime - lastTime) * 0.001f;

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				std::tstring temp = IntegerToString(DWORD(1/timeDelta));
				font->setString(temp.c_str());
				Render();

				lastTime = curTime;
			}


			while ((timer.getCurrentTime() - curTime) < 30);
		}
	}

	SAFE_DELETE(mesh);
	SAFE_DELETE(font);

	UnregisterClass(_T("D3D Tutorial"), wc.hInstance);
	return 0;
}



