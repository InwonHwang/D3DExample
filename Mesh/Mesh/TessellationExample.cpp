
#include "stdafx.h"
#include "Device.h"
#include "ZCamera.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

ID3DXMesh*                      SourceMesh = 0;
ID3DXMesh*                      PatchMesh = 0;
ID3DXMesh*                      TempMesh = 0;
ID3DXBuffer*					adjBuffer = 0;
ID3DXPatchMesh*					PatchMesh2 = 0;
std::vector<D3DMATERIAL9>       Mtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);
D3DMATERIAL9					mtrl;
float Segs = 1.5f;

ZCamera* Camera = NULL;
DWORD g_dwMouseX = 0;
DWORD g_dwMouseY = 0;


void InitMatrix()
{
	D3DXMATRIX matView;
	D3DXVECTOR3 vEyePt(0.0f, 20.0f, 1.0f);
	D3DXVECTOR3 vLookAtPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookAtPt, &vUpVec);
	D3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),
		800 / 600,
		1.0f,
		10000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection);

	float index = 1.5f;
	D3DXMATRIX matRotateY;
	D3DXMatrixRotationY(&matRotateY, index);
	D3DDevice->SetTransform(D3DTS_WORLD, &(matRotateY));

	Camera->SetView(&vEyePt, &vLookAtPt, &vUpVec);
}

void InitMesh(void)
{
	HRESULT hr = 0;
	
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	hr = D3DXLoadMeshFromX(_T("Hanzo.x"), D3DXMESH_SYSTEMMEM, D3DDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &SourceMesh);

	if (FAILED(hr)) DebugError(hr);

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			Mtrls.push_back(mtrls[i].MatD3D);
			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(D3DDevice, (TCHAR *)mtrls[i].pTextureFilename, &tex);
				Textures.push_back(tex);
			}
			else
				Textures.push_back(0);

		}
	}
	SAFE_RELEASE(mtrlBuffer);

	hr = SourceMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(), (DWORD*)adjBuffer->GetBufferPointer(), 0, 0);		
	if (FAILED(hr)) DebugError(hr);	

	SAFE_RELEASE(adjBuffer);

	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	mtrl.Ambient = D3DXCOLOR(255, 255, 0, 255);
	mtrl.Diffuse = D3DXCOLOR(255, 255, 0, 255);
	mtrl.Specular = D3DXCOLOR(255, 255, 0, 255);
	mtrl.Emissive = D3DXCOLOR(0, 0, 0, 255);
	mtrl.Power = 2.0f;
}

void GeneratePatchMesh(float seg)
{
	SAFE_RELEASE(PatchMesh);

	HRESULT hr = SourceMesh->CloneMeshFVF(D3DXMESH_WRITEONLY | D3DXMESH_NPATCHES | (SourceMesh->GetOptions() & D3DXMESH_32BIT),
		SourceMesh->GetFVF(), D3DDevice, &PatchMesh);
	if (FAILED(hr)) DebugError(hr);

	TempMesh = PatchMesh;

	//CPU Ã³¸®
	/*SAFE_RELEASE(PatchMesh);
	HRESULT hr = D3DXTessellateNPatches(SourceMesh, (DWORD*)adjBuffer->GetBufferPointer(), seg, false, &PatchMesh, NULL);
	if (FAILED(hr)) DebugError(hr);*/
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
	if (GetAsyncKeyState('W'))
	{
		Camera->MoveLocalZ(0.5f);
		if (Segs < 1.9f) Segs += 0.05f;
	}
	if (GetAsyncKeyState('S'))
	{
		Camera->MoveLocalZ(-0.5f);		
		if (Segs > 1.1f) Segs -= 0.05f;
	}
	if (GetAsyncKeyState('A')) Camera->MoveLocalX(-0.5f);
	if (GetAsyncKeyState('D')) Camera->MoveLocalX(0.5f);
	if (GetAsyncKeyState('Z')) Camera->RotateLocalX(0.01f);
	if (GetAsyncKeyState('X')) Camera->RotateLocalY(0.01f);
}

void ProcessInputs(void)
{
	ProcessMouse();
	ProcessKey();
}

void Render(void)
{
	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (D3DDevice->BeginScene());
	{
		for (int i = 0; i < Mtrls.size(); i++)
		{
			//// draw wireframe outline
			D3DDevice->SetMaterial(&mtrl);
			D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			//PatchMesh->DrawSubset(i);
			TempMesh->DrawSubset(i);
			D3DDevice->SetNPatchMode(0);
			D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			// draw pmesh
			/*D3DDevice->SetMaterial(&Mtrls[i]);
			D3DDevice->SetTexture(0, Textures[i]);
			SourceMesh->DrawSubset(i);*/
		}

		D3DDevice->EndScene();
	}

	D3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

void cleanD3D(void)
{
	SAFE_DELETE(Camera);
	SAFE_RELEASE(PatchMesh2);
	SAFE_RELEASE(SourceMesh);
	SAFE_RELEASE(PatchMesh);
	SAFE_RELEASE(TempMesh);

	for (int i = 0; i < Textures.size(); i++)
		SAFE_RELEASE(Textures[i]);

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
	InitMesh();
	InitLight();
	GeneratePatchMesh(Segs);

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
				
		//ProcessInputs();		
		GeneratePatchMesh(Segs);
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