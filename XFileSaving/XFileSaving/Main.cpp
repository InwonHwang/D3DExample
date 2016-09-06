#include <d3d9.h>
#include <d3dx9.h>
#include <dxfile.h>
#include <rmxfguid.h>
#include <rmxftmpl.h>
#include <cstdio>

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


void initD3D(HWND hWnd);
void cleanD3D(void);
void SaveTest(void);


//struct CUSTOMVERTEX {FLOAT X, Y, Z; D3DVECTOR NORMAL; FLOAT U, V;};
//#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
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
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);
	SaveTest();
	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;


		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
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


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
}

void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();
}

void SaveTest(void)
{

	ID3DXFile* xFile = NULL;
	HRESULT hr = D3DXFileCreate(&xFile);

	hr = xFile->RegisterTemplates((void*)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
	//xFile->RegisterTemplates((void*)XSKINEXP_TEMPLATES, strlen(XSKINEXP_TEMPLATES));

	ID3DXFileSaveObject* xFileSave = NULL;
	//xFile->CreateSaveObject("test.x", D3DXF_FILESAVE_TOFILE, D3DXF_FILEFORMAT_BINARY | DXFILEFORMAT_COMPRESSED, &xFileSave);
	xFile->CreateSaveObject("test.x", D3DXF_FILESAVE_TOFILE, D3DXF_FILEFORMAT_TEXT, &xFileSave);

	ID3DXFileSaveData *xFileSaveRoot = NULL;
	hr = xFileSave->AddDataObject(TID_D3DRMFrame, "Scene_Root_Frame", NULL, 0, NULL, &xFileSaveRoot);

	DWORD size = 16 * sizeof(float);

	D3DXMATRIX matWorld(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);	
	LPD3DXMESH mesh;

	D3DXLoadMeshFromX(L"tiny.x", D3DXMESH_MANAGED, d3ddev, NULL, NULL, NULL, NULL, &mesh);

	ID3DXFileSaveData *pRootTransform = NULL;
	xFileSaveRoot->AddDataObject(TID_D3DRMFrameTransformMatrix,
		"Root_Transform_Matrix", NULL, size, matWorld, &pRootTransform);

	for (int i = 0; i < 10; i++)
	{		
		char buf[256];
		sprintf_s(buf, "MeshFrame_%d", i);

		ID3DXFileSaveData *xFileMeshFrame = NULL;
		hr = xFileSaveRoot->AddDataObject(TID_D3DRMFrame, buf, NULL, 0, NULL, &xFileMeshFrame);

		
		LPD3DXBUFFER buffer;
		mesh->LockVertexBuffer(0, (LPVOID *)&buffer);

		ID3DXFileSaveData *xFileMeshFrame = NULL;
		hr = xFileSaveRoot->AddDataObject(TID_D3DRMFrame, buf, NULL, 0, buffer, &xFileMeshFrame);


		xFileMeshFrame->Release();
	}

	xFileSave->Save();

	mesh->Release();
	xFileSaveRoot->Release();
	xFileSave->Release();
	xFile->Release();
}