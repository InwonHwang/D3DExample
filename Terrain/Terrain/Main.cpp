
#include "stdafx.h"
#include "Device.h"
#include "ZCamera.h"
#include "MyTerrain.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


ZCamera* Camera = NULL;
MyTerrain* Terrain = 0;
DWORD g_dwMouseX = 0;
DWORD g_dwMouseY = 0;


void InitMatrix()
{
	D3DXMATRIX matView;
	D3DXVECTOR3 vEyePt(0.0f, 100.0f, 1.0f);
	D3DXVECTOR3 vLookAtPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookAtPt, &vUpVec);
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

	Camera->SetView(&vEyePt, &vLookAtPt, &vUpVec);
}

void InitLight(void)
{
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	D3DVECTOR vecDirection = { 0.0f, 0.0f, -1.0f };
	light.Direction = vecDirection;
	D3DDevice->SetLight(0, &light);
	D3DDevice->LightEnable(0, TRUE);

	return;
}

void ProcessMouse(void)
{
	POINT pt;
	float fDelta = 0.001f;

	GetCursorPos(&pt);
	int dx = pt.x - g_dwMouseX;
	int dy = pt.y - g_dwMouseY;

	Camera->RotateLocalX(dy * fDelta);
	Camera->RotateLocalY(dx * fDelta);
	D3DXMATRIXA16* pmatView = Camera->GetViewMatrix();
	D3DDevice->SetTransform(D3DTS_VIEW, pmatView);

	RECT rc;
	GetClientRect(GetActiveWindow(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(GetActiveWindow(), &pt);
	SetCursorPos(pt.x, pt.y);
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
}

void ProcessKey(void)
{
	if (GetAsyncKeyState('W')) Camera->MoveLocalZ(0.5f);
	if (GetAsyncKeyState('S'))Camera->MoveLocalZ(-0.5f);	
	if (GetAsyncKeyState('A')) Camera->MoveLocalX(-0.5f);
	if (GetAsyncKeyState('D')) Camera->MoveLocalX(0.5f);
	if (GetAsyncKeyState('Z')) Camera->RotateLocalX(0.01f);
	if (GetAsyncKeyState('X')) Camera->RotateLocalY(0.01f);
}

void Render(void)
{
	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);


	if (D3DDevice->BeginScene());
	{
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);

		if (Terrain) Terrain->draw(&I, false);
			

		D3DDevice->EndScene();
	}

	D3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

void cleanD3D(void)
{
	SAFE_DELETE(Camera);
	SAFE_DELETE(Terrain);
	D3D->Release();

	return;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = _T("WindowClass1");

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, _T("WindowClass1"), _T("Our Direct3D Program"),
		WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	SetActiveWindow(hWnd);
	D3D->Init();
	Camera = new ZCamera;	
	InitMatrix();
	InitLight();
	Terrain = new MyTerrain("coastMountain64.raw", 64, 64, 1, 0.1f);
	D3DXVECTOR3 lightDirection(0.0f, 1.0f, 0.0f);
	Terrain->genTexture(&lightDirection);
	//Terrain->loadTexture(_T("grass.bmp"));

	MSG msg;

	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
				
		ProcessMouse();
		ProcessKey();
		Render();

		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		while ((GetTickCount() - starting_point) < 25);
	}

	cleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}