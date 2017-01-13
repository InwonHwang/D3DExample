#include "Core.h"
#include "FbxTest.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;

bool Init()
{
	if (!FbxTest::Init()) return false;
	if (!FbxTest::ImportScene()) return false;
	FbxTest::Load();
		
	return true;
}

HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}


	return S_OK;
}

void InitMatrix()
{
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	D3DVECTOR vecDirection = { 0,0,0 };
	light.Direction = vecDirection;
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);	

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	D3DXVECTOR3 eye(0, 50, -1500);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 lookAt(0, 50, 0);


	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &eye, &lookAt, &up);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), 1.0f, 1.0f, 50000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}




VOID Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(32, 64, 128), 1.0f, 0);


	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		
		for (auto it = FbxTest::g_bones.begin(); it != FbxTest::g_bones.end(); ++it)
		{
			//(*it)->Draw(*g_pd3dDevice);			
		}

		//auto itMeshs = FbxTest::g_staticMeshs.begin();
		auto itMeshs = FbxTest::g_skinnedMeshs.begin();
		auto itFrames = FbxTest::g_frames.begin();		

		for (int i = 0; i < FbxTest::g_skinnedMeshs.size(); i++)
		{
			//g_pd3dDevice->SetTransform(D3DTS_WORLD, &(*itFrames)->GetWorldMatrix());
			(*itMeshs)->ApplyMatrix((*itFrames)->GetLocalMatrix(), (*itFrames)->GetWorldMatrix());
			(*itMeshs)->Draw();	

			itMeshs++;
			itFrames++;
		}

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




VOID Cleanup()
{
	FbxTest::Clear();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
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
		WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	SetActiveWindow(hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	

	if (SUCCEEDED(InitD3D(hWnd)))
	{	
		if(Init())
		{ 
			InitMatrix();
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();

				if (KEY_DOWN(VK_ESCAPE))
					PostMessage(hWnd, WM_DESTROY, 0, 0);
			}
		}
		
	}
	Cleanup();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		//Cleanup();
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}