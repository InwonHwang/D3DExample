#include "Core\Core.h"
#include "D3D\ResourceBase\D3DResourceBase.h"
#include "D3D\Component\D3DComponent.h"
#include "D3D\Device.h"
#include "D3D\Frustum.h"

#define MOVESPEED 0.5f
#define ROTATIONSPEED 2.0f

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Timer appTimer;

ResourceManager resourceManager;

IDirect3DDevice9* device;
FbxManager* fbxManager;

FBXDATASET fbxDataSet;
FBXParser fbxParser;

sp<Texture> textureMesh;

sp<Effect> effect;
sp<SurfaceMaterial> material;
sp<SurfaceMaterial> materialMesh;
sp<TerrainData> terrainData;
sp<SkinnedMesh> mesh;
sp<Animation> animation;


sp<Transform> transformCamera1;
//sp<Transform> transformTerrain;
sp<Transform> transformMesh;

sp<Camera> camera1;
sp<Frustum> frustum;

std::vector<sp<FBXBONEDATA>> boneDataVec;
std::vector<D3DXMATRIX> matLocalVec;
std::vector<D3DXMATRIX> matWorldVec;

int cam = 1;

void SetRenderState()
{
	Device::Instance()->Get()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->LightEnable(0, true);
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
	appTimer.SetUp();
	appTimer.Start();

	Device::Instance()->Init();
	device = Device::Instance()->Get();

	FBXMgr::Instance()->Init();
	fbxManager = FBXMgr::Instance()->Get();

	bool t = fbxParser.Init(*fbxManager, _T(""));
	fbxParser.Load(fbxDataSet);
	sp<FBXMESHDATA> meshData;
	for (auto data : fbxDataSet.fbxDataVec)
	{
		if (data->dataType == FBXDATA::eMesh)
			meshData = boost::static_pointer_cast<FBXMESHDATA>(data);
		else if (data->dataType == FBXDATA::eBone)
		{
			sp<FBXBONEDATA> temp = boost::static_pointer_cast<FBXBONEDATA>(data);
			boneDataVec.push_back(temp);		
			if (temp->pAnimation)
			{
				D3DXMATRIX templocal = FbxDXUtil::ToDXMatrix(temp->local);
				D3DXMATRIX tempworld = FbxDXUtil::ToDXMatrix(temp->globalBindposeInverse);
				matLocalVec.push_back(templocal);
				matWorldVec.push_back(tempworld);
			}
		}
	}

	// texture »ý¼º

	textureMesh = resourceManager.Create<Texture>();
	textureMesh->CreateTexture(*device, _T("Media\\Texture\\hero_color.bmp"));

	effect = resourceManager.Create<Effect>();
	effect->CreateEffect(*device, _T("Media\\Effect\\TextureMapping.fx"));

	material = resourceManager.Create<SurfaceMaterial>();
	material->SetEffect(effect);

	materialMesh = resourceManager.Create<SurfaceMaterial>();
	materialMesh->SetEffect(effect);

	terrainData = resourceManager.Create<TerrainData>();
	terrainData->Create(*device, _T("Media\\Terrain\\coastMountain64.raw"));

	mesh = resourceManager.Create<SkinnedMesh>();
	mesh->Create(*device, meshData);

	animation = resourceManager.Create<Animation>();
	animation->Create(boneDataVec);

	
	//Transform
	Transform* trc1 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformCamera1.reset(trc1, Memory<Transform>::OrderedFree);
	transformCamera1->SetLocalPosition(Vector3(0, 500, 0));
	transformCamera1->SetLocalRotation(Quaternion::Euler(90, 0, 0));
	/*transformCamera1->SetLocalPosition(Vector3(0, 0, -500));
	transformCamera1->SetLocalRotation(Quaternion::Euler(0, 0, 0));*/
	camera1 = transformCamera1->AddComponent<Camera>();
	camera1->SetTransform(transformCamera1);

	////Terrain
	//Transform* trt = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	//transformTerrain.reset(trt, Memory<Transform>::OrderedFree);
	//transformTerrain->AddComponent<Terrain>()->SetTerrainData(terrainData);
	//transformTerrain->SetLocalPosition(Vector3(0, 0, 0));

	//Mesh Renderer
	Transform* trm = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformMesh.reset(trm, Memory<Transform>::OrderedFree);
	transformMesh->AddComponent<SkinnedMeshRenderer>()->SetMesh(mesh);
	transformMesh->GetComponent<SkinnedMeshRenderer>()->SetMaterialCount(1);
	transformMesh->GetComponent<SkinnedMeshRenderer>()->SetMaterial(0, materialMesh);
	transformMesh->GetComponent<SkinnedMeshRenderer>()->SetBone(boneDataVec);
		
	frustum.reset(new Frustum);

	SetRenderState();
}

void RenderBone()
{
	for (int i = 0; i < animation->GetAnimCurveVec()->size(); ++i)
	{
		BOXVERTEX vtx[8];
		vtx[0] = BOXVERTEX(-2, 2, 2, 0xffff0000);		/// v0
		vtx[1] = BOXVERTEX(2, 2, 2, 0xffff0000);		/// v1
		vtx[2] = BOXVERTEX(2, 2, -2, 0xffff0000);		/// v2
		vtx[3] = BOXVERTEX(-2, 2, -2, 0xffff0000);		/// v3
		vtx[4] = BOXVERTEX(-2, -2, 2, 0xffff0000);		/// v4
		vtx[5] = BOXVERTEX(2, -2, 2, 0xffff0000);		/// v5
		vtx[6] = BOXVERTEX(2, -2, -2, 0xffff0000);		/// v6
		vtx[7] = BOXVERTEX(-2, -2, -2, 0xffff0000);		/// v7

		INDEX idx[12] =
		{
			{ 0, 1, 2 } ,{ 0, 2, 3 } ,	/// À­¸é
			{ 4, 6, 5 } ,{ 4, 7, 6 } ,	/// ¾Æ·§¸é
			{ 0, 3, 7 } ,{ 0, 7, 4 } ,	/// ¿Þ¸é
			{ 1, 5, 6 } ,{ 1, 6, 2 } ,	/// ¿À¸¥¸é
			{ 3, 2, 6 } ,{ 3, 6, 7 } ,	/// ¾Õ¸é
			{ 0, 4, 5 } ,{ 0, 5, 1 } 	/// µÞ¸é
		};

		appTimer.Update();
		uint time = appTimer.GetElapsedTime();

		int frame = time % animation->GetAnimCurveVec()->data()[i]->GetLength();

		D3DXMATRIX m;
		animation->GetAnimCurveVec()->data()[i]->GetAnimatedMatrix(frame, m);		

		device->SetTransform(D3DTS_WORLD, &m);
		device->SetFVF(BOXVERTEX::FVF);
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, idx, D3DFMT_INDEX32, vtx, sizeof BOXVERTEX);
	}
}

void Render()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);

	if (SUCCEEDED(device->BeginScene()))
	{

		transformCamera1->UpdateWorldMatrix();
		camera1->UpdateViewMatrix();
		camera1->Update(*device);


		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		device->SetTransform(D3DTS_WORLD, &world);
		frustum->Make(camera1->GetViewMatrix() * camera1->GetProjMatrix());
		//frustum->Draw(*device);
		
		RenderBone();

		material->SetMatrix(_T("gViewMatrix"), camera1->GetViewMatrix());
		material->SetMatrix(_T("gProjectionMatrix"), camera1->GetProjMatrix());
		material->SetTexture(_T("DiffuseMap_Tex"), textureMesh);

		
		transformMesh->UpdateWorldMatrix();
		material->SetMatrix(_T("gWorldMatrix"), transformMesh->GetMatrix());
		transformMesh->Update(*device);
		transformMesh->GetComponent<SkinnedMeshRenderer>()->Draw(*device);
		
		//transformTerrain->UpdateWorldMatrix();
		//transformTerrain->Update(*device);
		//transformTerrain->GetComponent<Terrain>()->Draw(*device);
		//transformTerrain->GetComponent<Terrain>()->DrawFrustum(*device, frustum);
		//transformTerrain->GetComponent<Terrain>()->DrawLOD(*device, frustum);

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
	transformCamera1->Destroy();
	//transformTerrain->Destroy();
	resourceManager.Destroy();

	Memory<Camera>::Clear();
	Memory<Transform>::Clear();
	Memory<Terrain>::Clear();
	Memory<MeshRenderer>::Clear();
	Memory<SkinnedMeshRenderer>::Clear();
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