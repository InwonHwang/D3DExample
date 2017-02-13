#include "Core\Core.h"
#include "D3D\ResourceBase\D3DResourceBase.h"
#include "D3D\Component\D3DComponent.h"
#include "D3D\Device.h"
#include "D3D\Frustum.h"

#define MOVESPEED 0.5f
#define ROTATIONSPEED 2.0f

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ResourceManager resourceManager;

IDirect3DDevice9* device;
FbxManager* fbxManager;

FBXData fbxData;
FBXParser fbxParser;

sp<Texture> texture;
sp<Sprite> sprite;
sp<Effect> effect;
sp<SurfaceMaterial> material;
sp<TerrainData> terrainData;

sp<Transform> transformSprite1;
sp<Transform> transformSprite2;
sp<Transform> transformCamera1;
sp<Transform> transformCamera2;
sp<Transform> transformTerrain;
sp<Camera> camera1;
sp<Camera> camera2;

sp<Frustum> frustum;

int cam = 1;

void SetRenderState()
{
	Device::Instance()->Get()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//Device::Instance()->Get()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device::Instance()->Get()->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device::Instance()->Get()->LightEnable(0, true);
}

void processInput()
{
	if (GetAsyncKeyState('W')) transformCamera1->SetLocalPosition(transformCamera1->GetLocalPosition() + Vector3(0, 0, MOVESPEED));
	if (GetAsyncKeyState('S')) transformCamera1->SetLocalPosition(transformCamera1->GetLocalPosition() + Vector3(0, 0, -MOVESPEED));
	if (GetAsyncKeyState('A')) transformCamera1->SetLocalPosition(transformCamera1->GetLocalPosition() + Vector3(-MOVESPEED, 0, 0));
	if (GetAsyncKeyState('D')) transformCamera1->SetLocalPosition(transformCamera1->GetLocalPosition() + Vector3(MOVESPEED, 0, 0));
	if (GetAsyncKeyState('R')) transformCamera1->SetLocalPosition(transformCamera1->GetLocalPosition() + Vector3(0, MOVESPEED, 0));
	if (GetAsyncKeyState('F')) transformCamera1->SetLocalPosition(transformCamera1->GetLocalPosition() + Vector3(0, -MOVESPEED, 0));

	if (GetAsyncKeyState('1'))
	{
		cam = 1;
	}
	if (GetAsyncKeyState('2'))
	{
		cam = 2;
	}

	if (GetAsyncKeyState('I'))
	{
		Vector3 euler = transformCamera1->GetLocalEulerAngle() + Vector3(-ROTATIONSPEED, 0, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(euler.x, euler.y, euler.z));
	}
	if (GetAsyncKeyState('K'))
	{
		Vector3 euler = transformCamera1->GetLocalEulerAngle() + Vector3(ROTATIONSPEED, 0, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(euler.x, euler.y, euler.z));
	}
	if (GetAsyncKeyState('J'))
	{
		Vector3 euler = transformCamera1->GetLocalEulerAngle() + Vector3(0, -ROTATIONSPEED, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(euler.x, euler.y, euler.z));
	}
	if (GetAsyncKeyState('L'))
	{
		Vector3 euler = transformCamera1->GetLocalEulerAngle() + Vector3(0, ROTATIONSPEED, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(euler.x, euler.y, euler.z));
	}	
}

void Init()
{
	Device::Instance()->Init();
	device = Device::Instance()->Get();

	FBXMgr::Instance()->Init();
	fbxManager = FBXMgr::Instance()->Get();

	bool t = fbxParser.Init(*fbxManager, _T(""));
	fbxParser.Load(fbxData);

	// texture »ý¼º
	texture = resourceManager.Create<Texture>();
	texture->CreateTexture(*device, _T("Media\\Texture\\Sample.bmp"));	

	effect = resourceManager.Create<Effect>();
	effect->CreateEffect(*device, _T("Media\\Effect\\TextureMapping.fx"));

	material = resourceManager.Create<SurfaceMaterial>();
	material->SetEffect(effect);

	terrainData = resourceManager.Create<TerrainData>();
	terrainData->Create(*device, _T("Media\\Terrain\\coastMountain64.raw"));

	sprite = resourceManager.Create<Sprite>();
	sprite->Create(*device, texture);

	Transform* trs1 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	Transform* trs2 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	Transform* trc1 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	Transform* trc2 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	Transform* trt = Memory<Transform>::OrderedAlloc(sizeof(Transform));

	transformSprite1.reset(trs1, Memory<Transform>::OrderedFree);
	transformSprite2.reset(trs2, Memory<Transform>::OrderedFree);
	transformCamera1.reset(trc1, Memory<Transform>::OrderedFree);
	transformCamera2.reset(trc2, Memory<Transform>::OrderedFree);
	transformTerrain.reset(trt, Memory<Transform>::OrderedFree);

	transformSprite1->AddComponent<SpriteRenderer>()->SetSprite(sprite);
	transformSprite1->GetComponent<SpriteRenderer>()->SetMaterialCount(1);
	transformSprite1->GetComponent<SpriteRenderer>()->SetMaterial(0, material);

	transformSprite2->AddComponent<SpriteRenderer>()->SetSprite(sprite);
	transformSprite2->GetComponent<SpriteRenderer>()->SetMaterialCount(1);
	transformSprite2->GetComponent<SpriteRenderer>()->SetMaterial(0, material);

	transformTerrain->AddComponent<Terrain>()->SetTerrainData(terrainData);


	camera1 = transformCamera1->AddComponent<Camera>();
	camera1->SetTransform(transformCamera1);

	camera2 = transformCamera2->AddComponent<Camera>();
	camera2->SetTransform(transformCamera2);

	transformSprite1->SetLocalPosition(Vector3(100, 100, 0));
	transformSprite2->SetLocalPosition(Vector3(-100, -100, 0));

	
	transformTerrain->SetLocalPosition(Vector3(0, 0, 0));
	//transformTerrain->SetLocalRotation(Quaternion::Euler(0, 180, 0));	

	transformCamera1->SetLocalPosition(Vector3(0, 90, 0));
	transformCamera1->SetLocalRotation(Quaternion::Euler(90, 0, 0));
	transformCamera2->SetLocalPosition(Vector3(0, 100, 0));
	transformCamera2->SetLocalRotation(Quaternion::Euler(90, 0, 0));
	
	frustum.reset(new Frustum);

	SetRenderState();
}


void Render()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (SUCCEEDED(device->BeginScene()))
	{
		if (cam == 1)
		{
			transformCamera1->UpdateWorldMatrix();
			camera1->UpdateViewMatrix();
			camera1->Update(*device);
		}
		else if (cam == 2)
		{
			transformCamera2->UpdateWorldMatrix();
			camera2->UpdateViewMatrix();
			camera2->Update(*device);
		}

		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		device->SetTransform(D3DTS_WORLD, &world);
		frustum->Make(camera1->GetViewMatrix() * camera1->GetProjMatrix());
		//frustum->Draw(*device);

		material->SetMatrix(_T("gViewMatrix"), camera1->GetViewMatrix());
		material->SetMatrix(_T("gProjectionMatrix"), camera1->GetProjMatrix());
		material->SetTexture(_T("DiffuseMap_Tex"), texture);
		
		transformTerrain->UpdateWorldMatrix();
		transformTerrain->Update(*device);
		//transformTerrain->GetComponent<Terrain>()->Draw(*device);
		//transformTerrain->GetComponent<Terrain>()->DrawFrustum(*device, frustum);
		transformTerrain->GetComponent<Terrain>()->DrawLOD(*device, frustum);

		/*transformSprite1->UpdateWorldMatrix();
		material->SetMatrix(_T("gWorldMatrix"), transformSprite1->GetMatrix());
		transformSprite1->Update(*device);		
		transformSprite1->GetComponent<SpriteRenderer>()->Draw(*device);

		transformSprite2->UpdateWorldMatrix();
		material->SetMatrix(_T("gWorldMatrix"), transformSprite2->GetMatrix());
		transformSprite2->Update(*device);		
		transformSprite2->GetComponent<SpriteRenderer>()->Draw(*device);*/

		device->EndScene();

	}

	device->Present(NULL, NULL, NULL, NULL);
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
		else
		{
			processInput();
			Render();
		}
			
	}

	frustum.reset();	
	transformSprite1->Destroy();
	transformSprite2->Destroy();
	transformCamera1->Destroy();
	transformCamera2->Destroy();
	transformTerrain->Destroy();
	resourceManager.Destroy();

	Memory<Camera>::Clear();
	Memory<Transform>::Clear();
	Memory<Terrain>::Clear();
	Memory<TransformImpl>::Clear();
	Memory<SpriteRenderer>::Clear();

	fbxParser.Release();
	FBXMgr::Instance()->Release();
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