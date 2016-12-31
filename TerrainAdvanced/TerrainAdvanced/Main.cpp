
#include "D3D\Device.h"
#include <d3dx9.h>
#include <vector>

#include "D3D\Method\Transform.h"
#include "D3D\Method\Camera.h"
#include "D3D\Frustum.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

DWORD					g_cxHeight = 0;
DWORD					g_czHeight = 0;
DWORD					g_dwMouseX = 0;
DWORD					g_dwMouseY = 0;
ID3DXMesh*                      SourceMesh = 0;
ID3DXBuffer*					adjBuffer = 0;

std::vector<D3DMATERIAL9>       Mtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);
D3DMATERIAL9					mtrl;

Transform t;
Transform t2;
Camera c;
Frustum f;

D3DXMATRIX			matProj;

void InitMatrix()
{	
	t.create();	
	
	/*t.setLocalPosition(Vector3(0, 0, 0));
	t.setLocalRotation(Vector3(0, 0, 0));*/
	
	t.update();

	/*DebugBox(0, floatToString(t.getEulerAngle().x).c_str());
	DebugBox(0, floatToString(t.getEulerAngle().y).c_str());
	DebugBox(0, floatToString(t.getEulerAngle().z).c_str());

	DebugBox(0, floatToString(t.getPosition().x).c_str());
	DebugBox(0, floatToString(t.getPosition().y).c_str());
	DebugBox(0, floatToString(t.getPosition().z).c_str());*/

	t2.create();
	t2.setLocalPosition(Vector3(0, 0, 80));
	t2.update();

	c.setTransform(t);
	c.update();

	f.create();
	f.setTransform(t);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0, 0, 0);
	D3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, 45 * Mathf::DegToRad(), 1.0f, 1.0f, 1000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixPerspectiveFovLH(&matProj, 45 * Mathf::DegToRad(), 1.0f, 1.0f, 200.0f);
	
}

void InitMesh(void)
{
	HRESULT hr = 0;

	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	hr = D3DXLoadMeshFromX(_T("Media\\airplane 2.x"), D3DXMESH_MANAGED, D3DDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &SourceMesh);


	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (UINT i = 0; i < numMtrls; i++)
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

	SAFE_RELEASE(adjBuffer);

	//D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	mtrl.Ambient = D3DXCOLOR(255, 255, 0, 255);
	mtrl.Diffuse = D3DXCOLOR(255, 255, 0, 255);
	mtrl.Specular = D3DXCOLOR(255, 255, 0, 255);
	mtrl.Emissive = D3DXCOLOR(0, 0, 0, 255);
	mtrl.Power = 2.0f;
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


void Render(void)
{
	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (D3DDevice->BeginScene())
	{
		for (UINT i = 0; i < Mtrls.size(); i++)
		{
			f.draw();

			// draw pmesh
			D3DDevice->SetMaterial(&Mtrls[i]);
			D3DDevice->SetTexture(0, Textures[i]);
			SourceMesh->DrawSubset(i);			
		}

		D3DDevice->EndScene();
	}

	D3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}


/**-----------------------------------------------------------------------------
* 키보드 입력 처리
*------------------------------------------------------------------------------
*/
void ProcessKey(void)
{
	static float posZ = 0;	
	if (GetAsyncKeyState('Q')) t.setLocalPosition(Vector3(0, 0, --posZ));	// 카메라 전진!
	if (GetAsyncKeyState('E')) t.setLocalPosition(Vector3(0, 0, ++posZ));	// 카메라 후진!

	static float rotX = 0;
	static float rotY = 0;
	static float rotZ = 0;
	if (GetAsyncKeyState('W')) t.setLocalRotation(Vector3(++rotX, rotY, rotZ));
	if (GetAsyncKeyState('S')) t.setLocalRotation(Vector3(--rotX, rotY, rotZ));
	if (GetAsyncKeyState('A')) t.setLocalRotation(Vector3(rotX, ++rotY, rotZ));
	if (GetAsyncKeyState('D')) t.setLocalRotation(Vector3(rotX, --rotY, rotZ));
	if (GetAsyncKeyState('Z')) t.setLocalRotation(Vector3(rotX, rotY, --rotZ));
	if (GetAsyncKeyState('X')) t.setLocalRotation(Vector3(rotX, rotY, ++rotZ));

	
	if (GetAsyncKeyState(VK_ESCAPE)) PostMessage(GetActiveWindow(), WM_DESTROY, 0, 0L);
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		c.setTransform(t2);
	}
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		c.setTransform(t);
	}
	D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/*if (GetAsyncKeyState(VK_LBUTTON)) D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (GetAsyncKeyState(VK_RBUTTON)) D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
	t.update();
	c.update();
	f.make(matProj);
}

/**-----------------------------------------------------------------------------
* 입력 처리
*------------------------------------------------------------------------------
*/
void ProcessInputs(void)
{
	ProcessKey();
}

void cleanD3D(void)
{
	SAFE_RELEASE(SourceMesh);

	for (UINT i = 0; i < Textures.size(); i++)
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
	InitMatrix();
	InitMesh();
	InitLight();

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
		
		ProcessInputs();
		Render();		
		

		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		while ((GetTickCount() - starting_point) < 25);
	}

	cleanD3D();
	t.destroy();
	t2.destroy();
	f.destroy();

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