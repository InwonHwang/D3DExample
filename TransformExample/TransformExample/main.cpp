// include the basic windows header files and the Direct3D header file
#include <d3d9.h>
#include <d3dx9.h>
#include "Device.h"
#include "IWEMesh.h"
#include "IWEGameObject.h"
#include "IWETransform.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
GameObject *obj;

void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void init_light(void);

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
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",
		L"Our Direct3D Program",
		WS_EX_TOPMOST | WS_POPUP,
		0, 0,
		640, 480,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	CDevice::GetInstance()->Init(hWnd);
	obj = new GameObject();
	init_graphics();
	init_light();
	
	

	// enter the main loop:

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

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		while ((GetTickCount() - starting_point) < 25);
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

// this is the function used to render a single frame
void render_frame(void)
{
	g_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);	
	g_Device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	g_Device->BeginScene();

	// SET UP THE TRANSFORMS

	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 8.0f, 16.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	g_Device->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		640 / 480,    // the aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane
	g_Device->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	static float time = 0;
	//obj->transform->setScale(IWEVector3(time, time, time));
	//obj->transform->setPosition(IWEVector3(time - 1, time - 1, time - 1));
	if (time < 30)	
		obj->transform->setRotation(IWEQuaternion::Euler(0, time, 0));
	
	obj->update();	
	time += 0.1f;
	g_Device->EndScene();

	g_Device->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	SAFE_DELETE(obj);
	CDevice::GetInstance()->Release();

	return;
}

// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	Mesh *tmpMesh = new Mesh();
	LPD3DXBUFFER bufShipMaterial;

	D3DXLoadMeshFromX(L"airplane 2.x",    // load this file
		D3DXMESH_SYSTEMMEM,    // load the mesh into system memory
		g_Device,    // the Direct3D Device
		NULL,    // we aren't using adjacency
		&bufShipMaterial,    // put the materials here
		NULL,    // we aren't using effect instances
		&tmpMesh->numMaterials,    // the number of materials in this model
		&tmpMesh->pMeshData->pMesh);    // put the mesh here


	// retrieve the pointer to the buffer containing the material information
	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufShipMaterial->GetBufferPointer();
	
	//// create a new material buffer and texture for each material in the mesh
	tmpMesh->materials = new D3DMATERIAL9[tmpMesh->numMaterials];
	tmpMesh->texture = new LPDIRECT3DTEXTURE9[tmpMesh->numMaterials];

	for (DWORD i = 0; i < tmpMesh->numMaterials; i++)    // for each material...
	{
		tmpMesh->materials[i] = tempMaterials[i].MatD3D;    // get the material info
		tmpMesh->materials[i].Ambient = tmpMesh->materials[i].Diffuse;    // make ambient the same as diffuse
													  // if there is a texture to load, load it

		if (FAILED(D3DXCreateTextureFromFileA(g_Device,
			tempMaterials[i].pTextureFilename,
			&tmpMesh->texture[i])))
			tmpMesh->texture[i] = NULL;    // if there is no texture, set the texture to NULL
	}

	RELEASE_COM(bufShipMaterial);
	obj->mesh = tmpMesh;

	
	return;
}

void init_light(void)
{
	D3DLIGHT9 light;    // create the light struct

	ZeroMemory(&light, sizeof(light));    // clear out the struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse.r = 0.5f;    // .5 red
	light.Diffuse.g = 0.5f;    // .5 green
	light.Diffuse.b = 0.5f;    // .5 blue
	light.Diffuse.a = 1.0f;    // full alpha (we'll get to that soon)

	D3DVECTOR vecDirection = { -1.0f, -0.3f, -1.0f };    // the direction of the light
	light.Direction = vecDirection;    // set the direction

	g_Device->SetLight(0, &light);    // send the light struct properties to light #0
	g_Device->LightEnable(0, TRUE);    // turn on light #0

	return;
}

