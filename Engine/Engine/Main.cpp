#include "Core\Core.h"
#include "D3D\ResourceBase\D3DResourceBase.h"
#include "D3D\Component\D3DComponent.h"
#include "D3D\Device.h"
#include "D3D\Frustum.h"

#define MOVESPEED 3
#define ROTATIONSPEED 2.0f

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Timer appTimer;

ResourceManager resourceManager;

IDirect3DDevice9* device;
FbxManager* fbxManager;

FBXDATASET fbxDataSet;
FBXParser fbxParser;

sp<Texture> textureMesh;
sp<Sprite> sprite;
sp<Effect> effect;
sp<SurfaceMaterial> material;
sp<TerrainData> terrainData;
sp<SkinnedMesh> mesh;
sp<StaticMesh> staticMesh;
sp<Animation> animation;


sp<Transform> transformCamera1;
//sp<Transform> transformTerrain;
sp<Transform> transformMesh;
sp<Transform> transformStaticMesh;
sp<Transform> transformSprite1;
sp<Transform> transformSprite2;

sp<Camera> camera1;
sp<Frustum> frustum;

std::vector<sp<FBXBONEDATA>> boneDataVec;

void SetRenderState()
{
	//Device::Instance()->Get()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->LightEnable(0, true);
}

void processInput()
{
	Vector3 camPos;
	transformCamera1->GetLocalPosition(camPos);
	if (GetAsyncKeyState('W')) transformCamera1->SetLocalPosition(camPos + Vector3(0, 0, MOVESPEED));
	if (GetAsyncKeyState('S')) transformCamera1->SetLocalPosition(camPos + Vector3(0, 0, -MOVESPEED));
	if (GetAsyncKeyState('A')) transformCamera1->SetLocalPosition(camPos + Vector3(-MOVESPEED, 0, 0));
	if (GetAsyncKeyState('D')) transformCamera1->SetLocalPosition(camPos + Vector3(MOVESPEED, 0, 0));
	if (GetAsyncKeyState('R')) transformCamera1->SetLocalPosition(camPos + Vector3(0, MOVESPEED, 0));
	if (GetAsyncKeyState('F')) transformCamera1->SetLocalPosition(camPos + Vector3(0, -MOVESPEED, 0));

	Vector3 eulerAngle;
	Vector3 resultAngle;
	transformCamera1->GetLocalEulerAngle(eulerAngle);
	if (GetAsyncKeyState('I'))
	{
		resultAngle = eulerAngle + Vector3(-ROTATIONSPEED, 0, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
	if (GetAsyncKeyState('K'))
	{		
		resultAngle = eulerAngle + Vector3(ROTATIONSPEED, 0, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
	if (GetAsyncKeyState('J'))
	{
		resultAngle = eulerAngle + Vector3(0, -ROTATIONSPEED, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
	if (GetAsyncKeyState('L'))
	{
		resultAngle = eulerAngle + Vector3(0, ROTATIONSPEED, 0);
		transformCamera1->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
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

	fbxParser.Init(*fbxManager, _T(""));
	fbxParser.Load(fbxDataSet);
	sp<FBXMESHDATA> meshData;
	for (auto data : fbxDataSet.fbxDataVec)
	{
		if (data->dataType == FBXDATA::eMesh)
		{
			meshData = boost::static_pointer_cast<FBXMESHDATA>(data);			
		}
		else if (data->dataType == FBXDATA::eBone)
		{
			sp<FBXBONEDATA> temp = boost::static_pointer_cast<FBXBONEDATA>(data);
			boneDataVec.push_back(temp);
		}
		else if (data->dataType == FBXDATA::eTransform)
		{
			sp<FBXTRANSFORMDATA> temp = boost::static_pointer_cast<FBXTRANSFORMDATA>(data);			
		}
	}
	
	// texture ����

	textureMesh = resourceManager.Create<Texture>();
	textureMesh->CreateTexture(*device, _T("Media\\Texture\\hero_color.bmp"));

	effect = resourceManager.Create<Effect>();
	effect->CreateEffect(*device, _T("Media\\Effect\\TextureMapping.fx"));

	material = resourceManager.Create<SurfaceMaterial>();
	material->SetEffect(effect);

	terrainData = resourceManager.Create<TerrainData>();
	terrainData->Create(*device, _T("Media\\Terrain\\coastMountain64.raw"));

	mesh = resourceManager.Create<SkinnedMesh>();
	mesh->Create(*device, meshData);

	staticMesh = resourceManager.Create<StaticMesh>();
	staticMesh->Create(*device, meshData);

	animation = resourceManager.Create<Animation>();
	animation->Create(boneDataVec);

	sprite = resourceManager.Create<Sprite>();
	sprite->Create(*device, textureMesh);

	//Transform
	Transform* trc1 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformCamera1.reset(trc1, Memory<Transform>::OrderedFree);
	transformCamera1->SetLocalPosition(Vector3(0, 0, -500));
	camera1 = transformCamera1->AddComponent<Camera>();
	camera1->SetTransform(transformCamera1);

	Transform* trs1 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformSprite1.reset(trs1, Memory<Transform>::OrderedFree);
	transformSprite1->AddComponent<SpriteRenderer>()->SetSprite(sprite);
	transformSprite1->GetComponent<SpriteRenderer>()->SetMaterialCount(1);
	transformSprite1->GetComponent<SpriteRenderer>()->SetMaterial(0, material);
	transformSprite1->SetLocalScale(Vector3(0.3f, 0.3f, 0.3f));
	transformSprite1->SetLocalPosition(Vector3(-50, 0, 0));

	Transform* trs2 = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformSprite2.reset(trs2, Memory<Transform>::OrderedFree);
	transformSprite2->AddComponent<SpriteRenderer>()->SetSprite(sprite);
	transformSprite2->GetComponent<SpriteRenderer>()->SetMaterialCount(1);
	transformSprite2->GetComponent<SpriteRenderer>()->SetMaterial(0, material);
	transformSprite2->SetLocalScale(Vector3(0.3f, 0.3f, 0.3f));
	transformSprite2->SetLocalPosition(Vector3(100, 0, 0));
	transformSprite2->SetParent(transformSprite1);

	////Terrain
	/*Transform* trt = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformTerrain.reset(trt, Memory<Transform>::OrderedFree);
	transformTerrain->AddComponent<Terrain>()->SetTerrainData(terrainData);
	transformTerrain->SetLocalPosition(Vector3(0, 0, 0));*/

	//Mesh Renderer
	Transform* trm = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformMesh.reset(trm, Memory<Transform>::OrderedFree);
	transformMesh->AddComponent<SkinnedMeshRenderer>()->SetMesh(mesh);
	transformMesh->GetComponent<SkinnedMeshRenderer>()->SetMaterialCount(1);
	transformMesh->GetComponent<SkinnedMeshRenderer>()->SetMaterial(0, material);
	transformMesh->GetComponent<SkinnedMeshRenderer>()->SetBone(boneDataVec);

	//transformStaticMesh
	Transform* trsm = Memory<Transform>::OrderedAlloc(sizeof(Transform));
	transformStaticMesh.reset(trm, Memory<Transform>::OrderedFree);
	transformStaticMesh->AddComponent<MeshRenderer>()->SetMesh(staticMesh);
	transformStaticMesh->GetComponent<MeshRenderer>()->SetMaterialCount(1);
	transformStaticMesh->GetComponent<MeshRenderer>()->SetMaterial(0, material);
		
	frustum.reset(new Frustum);	

	SetRenderState();
}

void RenderBone()
{
	for (uint i = 0; i < animation->GetAnimCurveVec()->size(); ++i)
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
			{ 0, 1, 2 } ,{ 0, 2, 3 } ,	/// ����
			{ 4, 6, 5 } ,{ 4, 7, 6 } ,	/// �Ʒ���
			{ 0, 3, 7 } ,{ 0, 7, 4 } ,	/// �޸�
			{ 1, 5, 6 } ,{ 1, 6, 2 } ,	/// ������
			{ 3, 2, 6 } ,{ 3, 6, 7 } ,	/// �ո�
			{ 0, 4, 5 } ,{ 0, 5, 1 } 	/// �޸�
		};

		appTimer.Update();
		uint time = appTimer.GetElapsedTime();

		int frame = time % animation->GetAnimCurveVec()->data()[i]->GetLength();
		D3DXMATRIX m;		

		//animation->GetAnimCurveVec()->data()[i]->GetAnimatedMatrix(frame, m);
		m = FbxDXUtil::ToDXMatrix(boneDataVec[i]->animVec[(frame - (frame%60))/60]->globalTransform);
		
		
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

		//RenderBone();

		//frustum->Make(camera1->GetViewMatrix() * camera1->GetProjMatrix());
		//frustum->Draw(*device);

		material->SetMatrix(_T("gViewMatrix"), camera1->GetViewMatrix());
		material->SetMatrix(_T("gProjectionMatrix"), camera1->GetProjMatrix());		
		material->SetTexture(_T("DiffuseMap_Tex"), textureMesh);
		
		/*transformSprite1->GetMatrixWorld(matWorld);
		transformSprite1->UpdateWorldMatrix();
		transformSprite1->Update(device);
		material->SetMatrix(_T("gWorldMatrix"), matWorld);
		transformSprite1->GetComponent<SpriteRenderer>()->Draw(*device);

		transformSprite2->GetMatrixWorld(matWorld);
		transformSprite2->UpdateWorldMatrix();
		transformSprite2->Update(device);
		material->SetMatrix(_T("gWorldMatrix"), matWorld);
		transformSprite2->GetComponent<SpriteRenderer>()->Draw(*device);*/
		
		appTimer.Update();
		uint time = appTimer.GetElapsedTime();
		int frame = time % animation->GetAnimCurveVec()->data()[0]->GetLength();

		D3DXMATRIX matWorld;
		//D3DXMatrixIdentity(&matWorld);
		transformMesh->GetMatrixWorld(matWorld);
		material->SetMatrix(_T("gWorldMatrix"), matWorld);
		//transformMesh->GetComponent<SkinnedMeshRenderer>()->Test(*device, animation, frame);
		transformMesh->GetComponent<SkinnedMeshRenderer>()->ApplyMatrix(animation, frame);
		transformMesh->GetComponent<SkinnedMeshRenderer>()->Draw(*device);

		//D3DXMATRIX matWorld;
		////D3DXMatrixIdentity(&matWorld);
		//transformStaticMesh->GetMatrixWorld(matWorld);
		//material->SetMatrix(_T("gWorldMatrix"), matWorld);
		//transformStaticMesh->GetComponent<MeshRenderer>()->Draw(*device);

		//transformMesh->GetComponent<SkinnedMeshRenderer>()->Test(*device, animation, frame);
		
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