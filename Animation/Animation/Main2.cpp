
#include "stdafx.h"
#include "Device.h"
#include "Pool.h"
#include "XFile.h"
#include "Frame.h"
#include "Animation.h"
#include <rmxfguid.h>

Animation *anim;
ID3DXAnimationController *pAnimationController;
D3DXMATRIX matrix;

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
		SAFE_DELETE(anim);
		SAFE_RELEASE(pAnimationController);
		D3D->Release();
		XFILE->Release();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
void parseXFileData(LPD3DXFILEDATA pXFileData);
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

	if (XFILE->Init())
	{
		D3DXCreateAnimationController(1, 1, 1, 1, &pAnimationController);
		LPD3DXFILEENUMOBJECT pEnumObject = NULL;
		HRESULT hr = 0;

		if (FAILED(hr = D3DXFILE->CreateEnumObject("tiny.x", DXFILELOAD_FROMFILE, &pEnumObject)))
		{
			DebugError(hr);
		}

		SIZE_T childCount = 0;
		pEnumObject->GetChildren(&childCount);

		for (SIZE_T i = 0; i < childCount; i++)
		{
			LPD3DXFILEDATA data = NULL;

			if (FAILED(hr = pEnumObject->GetChild(i, &data)))
			{
				DebugError(hr);
				break;
			}
					
			
			parseXFileData(data);
			

			SAFE_RELEASE(data);
		}
	}

	if (D3D->Init())
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	

	UnregisterClass(_T("D3D Tutorial"), wc.hInstance);
	return 0;
}

void parseXFileData(LPD3DXFILEDATA pXFileData)
{
	HRESULT hr = 0;
	GUID type;

	hr = pXFileData->GetType(&type);

	//Frame* frame = new Frame;
	
	if (type == TID_D3DRMFrameTransformMatrix)
	{		
	}
	else if (type == TID_D3DRMAnimationSet)
	{	
		Animation *anim = new Animation();
		
		anim->load(pXFileData);
		anim->load("Test", 1, true);

		pAnimationController->RegisterAnimationSet(anim->GetAnimationSet());
		pAnimationController->RegisterAnimationOutput("Bip01", &matrix, 0, 0, 0);
		pAnimationController->SetTrackAnimationSet(0, anim->GetAnimationSet());

		DebugBox(0, floatToString(matrix._11).c_str());

		D3DXTRACK_DESC Desc;
		Desc.Enable = true;
		Desc.Speed = 1;
		Desc.Weight = 1;
		Desc.Position = 0;
		Desc.Priority = D3DXPRIORITY_HIGH;


		pAnimationController->SetTrackDesc(0, &Desc);
		pAnimationController->ResetTime();		
		pAnimationController->AdvanceTime(160, 0);

		DebugBox(0, floatToString(matrix._11).c_str());

	}
	
	
	
	//SAFE_DELETE(frame);

	SIZE_T childCount;	
	pXFileData->GetChildren(&childCount);

	for (SIZE_T i = 0; i < childCount; i++)
	{
		LPD3DXFILEDATA pSubData;
		if (FAILED(hr = pXFileData->GetChild(i, &pSubData)))
			continue;

		parseXFileData(pSubData);
		SAFE_RELEASE(pSubData);
	}
}


