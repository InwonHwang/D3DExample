#include "Core\Core.h"
#include "D3D\Resource\D3DResource.h"
#include "D3D\Component\D3DComponent.h"
#include "D3D\Device.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ResourcePool<Texture> g_TexturePool(0);

sp<Texture> texture;
wp<Image> image;
sp<Transform> transform;

void Init()
{
	Device::Instance()->Init();

	texture = g_TexturePool.Create();
	texture->LoadTexture(*Device::Instance()->GetD3DDevice(), _T("Media\\Sample.bmp"));

	Transform* tr = (Transform *)Memory<Transform>::OrderedAlloc(sizeof(Transform));	
	transform.reset(tr, Memory<Transform>::OrderedFree);

	transform->AddComponent<Image>();
	image = transform->GetComponent<Image>();
	image.lock()->Create(*Device::Instance()->GetD3DDevice(), texture);
}

void Render()
{
	Device::Instance()->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	Device::Instance()->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (SUCCEEDED(Device::Instance()->GetD3DDevice()->BeginScene()))
	{		
		//transform->GetComponent<Image>()->Draw(*Device::Instance()->GetD3DDevice());
		image.lock()->Draw(*Device::Instance()->GetD3DDevice());
		Device::Instance()->GetD3DDevice()->EndScene();
	}

	Device::Instance()->GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
}


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
	Init();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Render();
	}
		

	transform->Clear();
	g_TexturePool.Clear();	
	Device::Instance()->Release();

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