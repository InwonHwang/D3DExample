
#include "D3D\Device.h"
#include <d3dx9.h>
#include <vector>

#include "D3D\Method\Transform.h"
#include "D3D\Method\Terrain.h"
#include "D3D\Method\Camera.h"
#include "D3D\Frustum.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//////////////////////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTexHeight = NULL; // Texture ���̸�
LPDIRECT3DTEXTURE9		g_pTexDiffuse = NULL; // Texture �����

struct MYINDEX
{
	WORD	topLeft, topRight, bottomLeft;		/// WORD, 16��Ʈ �ε���
};
//////////////////////////////////////////////////////////////////////////////////////////////////

Transform t;
Transform t2;

Terrain* terrain;
Camera c;

Frustum f;

D3DXMATRIX			matProj;

bool render;

/**-----------------------------------------------------------------------------
* �ؽ�ó �ʱ�ȭ
*------------------------------------------------------------------------------
*/
HRESULT InitTexture()
{	
	if (FAILED(D3DXCreateTextureFromFileEx(D3DDevice, "Media/map128.bmp",
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &g_pTexHeight)))
		return E_FAIL;

	// �����
	if (FAILED(D3DXCreateTextureFromFile(D3DDevice, "Media/tile2.tga", &g_pTexDiffuse)))
		return E_FAIL;

	return S_OK;
}

void InitMatrix()
{	
	if (FAILED(InitTexture())) return;
	terrain = new Terrain(1, 0.1f);
	terrain->create(D3DDevice, g_pTexHeight);


	t.create();		
	t.setLocalRotation(Quaternion::Euler(0, 0, 0));
	t.setLocalPosition(Vector3(0, 0, 50));
	t.update();

	t2.create();
	t2.setLocalPosition(Vector3(0, 140, 200));
	t2.setLocalRotation(Quaternion::Euler(-45, 0, 0));
	t2.update();

	c.setTransform(t);
	c.update();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	f.create();
	f.setTransform(t);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, 45 * Mathf::DegToRad(), 1.0f, 1.0f, 10000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void InitLight(void)
{
	/// ����(material)����
	/// ������ ����̽��� �� �ϳ��� ������ �� �ִ�.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	D3DDevice->SetMaterial(&mtrl);

	/// ���� ����
	D3DXVECTOR3 vecDir;									/// ���⼺ ����(directional light)�� ���� ���� ����
	D3DLIGHT9 light;									/// ���� ����ü
	ZeroMemory(&light, sizeof(D3DLIGHT9));			/// ����ü�� 0���� �����.
	light.Type = D3DLIGHT_DIRECTIONAL;			/// ������ ����(�� ����,���⼺ ����,����Ʈ����Ʈ)
	light.Diffuse.r = 1.0f;							/// ������ ����� ���
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	vecDir = D3DXVECTOR3(1, 1, 1);					/// ���� ����
	vecDir = D3DXVECTOR3(cosf(GetTickCount() / 350.0f),	/// ���� ȸ��
		1.0f,
		sinf(GetTickCount() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	/// ������ ������ �������ͷ� �����.
	light.Range = 1000.0f;									/// ������ �ٴٸ��� �ִ� �ִ�Ÿ�
	D3DDevice->SetLight(0, &light);							/// ����̽��� 0�� ���� ��ġ
	D3DDevice->LightEnable(0, true);							/// 0�� ������ �Ҵ�
	D3DDevice->SetRenderState(D3DRS_LIGHTING, true);			/// ���������� �Ҵ�

	D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00909090);		/// ȯ�汤��(ambient light)�� �� ����
}

/**-----------------------------------------------------------------------------
* Ű���� �Է� ó��
*------------------------------------------------------------------------------
*/
void ProcessKey(void)
{
	static float posX = 0;
	static float posY = 0;
	static float posZ = 50;

	if (GetAsyncKeyState('W')) t.setLocalPosition(Vector3(posX, posY, --posZ));	// ī�޶� ����!
	if (GetAsyncKeyState('S')) t.setLocalPosition(Vector3(posX, posY, ++posZ));	// ī�޶� ����!
	if (GetAsyncKeyState('D')) t.setLocalPosition(Vector3(--posX, posY, posZ));	// ī�޶� ����!
	if (GetAsyncKeyState('A')) t.setLocalPosition(Vector3(++posX, posY, posZ));	// ī�޶� ����!
	if (GetAsyncKeyState('R')) t.setLocalPosition(Vector3(posX, ++posY, posZ));	// ī�޶� ����!
	if (GetAsyncKeyState('F')) t.setLocalPosition(Vector3(posX, --posY, posZ));	// ī�޶� ����!

	static float rotX = 0;
	static float rotY = 0;
	static float rotZ = 0;
	if (GetAsyncKeyState('K')) t.setLocalRotation(Quaternion::Euler(--rotX, rotY, rotZ));
	if (GetAsyncKeyState('I')) t.setLocalRotation(Quaternion::Euler(++rotX, rotY, rotZ));
	if (GetAsyncKeyState('L')) t.setLocalRotation(Quaternion::Euler(rotX, ++rotY, rotZ));
	if (GetAsyncKeyState('J')) t.setLocalRotation(Quaternion::Euler(rotX, --rotY, rotZ));


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
		
	
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, 45 * Mathf::DegToRad(), 1.0f, 1.0f, 250);	

	D3DXMATRIX m;
	D3DXMatrixMultiply(&m, &t.getMatrix(), &matProj);

	f.make(m);
}


void ProcessInputs(void)
{
	ProcessKey();
}


void Render(void)
{
	D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (D3DDevice->BeginScene())
	{
		
		f.draw();

		if (terrain) terrain->draw(D3DDevice, f);		

		D3DDevice->EndScene();
	}

	D3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

void cleanD3D(void)
{

	SAFE_DELETE(terrain);

	if (g_pTexHeight != NULL)
		g_pTexHeight->Release();

	if (g_pTexDiffuse != NULL)
		g_pTexDiffuse->Release();

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
	InitLight();

	MSG msg;

	while (true)
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