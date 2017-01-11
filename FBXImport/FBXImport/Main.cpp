#include "Core.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "Frame.h"
#include "AnimationCurve.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
SkinnedMesh*			mesh[6];
Frame*					frame[6];

std::vector<Frame *>* bone;


bool fbxSdk();
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

	D3DVECTOR vecDirection = { 83,20,16 };
	light.Direction = vecDirection;
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);	

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	/*D3DXVECTOR3 eye(-83, 20, -5000);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 lookAt(-83,20,16);*/
	
	D3DXVECTOR3 eye(0, 50, -500);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 lookAt(0, 50, 0);


	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &eye, &lookAt, &up);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), 1.0f, 1.0f, 50000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void LoadBoneHierarachy(FbxNode *pNode, std::vector<Frame *>& boneHierarchy)
{	
	FbxNodeAttribute::EType AttributeType = pNode->GetNodeAttribute()->GetAttributeType();

	if (AttributeType == FbxNodeAttribute::eSkeleton)
	{
		Frame* tempBone = new Frame();

		tempBone->Load(*pNode, nullptr);
		boneHierarchy.push_back(tempBone);
	}

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		LoadBoneHierarachy(pNode->GetChild(i), boneHierarchy);
	}
}

bool fbxSdk()
{
	// fbx 싱글톤
	FbxManager* fbxManager = FbxManager::Create();

	// IO 세팅
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	ios->SetBoolProp(IMP_FBX_LINK, true);
	ios->SetBoolProp(IMP_FBX_SHAPE, true);
	ios->SetBoolProp(IMP_FBX_GOBO, true);
	ios->SetBoolProp(IMP_FBX_ANIMATION, true);
	ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	ios->SetBoolProp(EXP_FBX_MATERIAL, true);
	ios->SetBoolProp(EXP_FBX_TEXTURE, true);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	ios->SetBoolProp(EXP_FBX_SHAPE, true);
	ios->SetBoolProp(EXP_FBX_GOBO, true);
	ios->SetBoolProp(EXP_FBX_ANIMATION, true);
	ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	fbxManager->SetIOSettings(ios);

	// Scene 임포트
	FbxImporter* importer = FbxImporter::Create(fbxManager, "");

	//const char* fileName = "Media\\lowpolytree.fbx";
	//const char* fileName = "Media\\Medieva_fantasy_house.fbx";
	//const char* fileName = "Media\\baum hd med fbx.fbx";
	const char* fileName = "Media\\Hero_General.fbx";

	bool importStatus = importer->Initialize(fileName, -1, fbxManager->GetIOSettings());

	if (!importStatus) {
		MessageBoxA(GetActiveWindow(), NULL, importer->GetStatus().GetErrorString(), MB_OK);
		importer->Destroy();
		fbxManager->Destroy();
		return false;
	}

	FbxScene* scene = FbxScene::Create(fbxManager, "Scene");

	importer->Import(scene);
	importer->Destroy();


	FbxNode* pFbxRootNode = scene->GetRootNode();

	int j = 0;
	if (pFbxRootNode)
	{
		/*int numStacks = scene->GetSrcObjectCount(FbxCriteria::ObjectType(FbxAnimStack::ClassId));

		FbxAnimStack* pAnimStack = FbxCast<FbxAnimStack>(scene->GetSrcObject(FbxCriteria::ObjectType(FbxAnimStack::ClassId), 0));
				

		int numAnimLayers = pAnimStack->GetMemberCount(FbxCriteria::ObjectType(FbxAnimLayer::ClassId));

		for (int i = 0; i < numAnimLayers; i++)
		{
			FbxAnimLayer* animLayer = FbxCast<FbxAnimLayer>(pAnimStack->GetMember(FbxCriteria::ObjectType(FbxAnimLayer::ClassId), i));
		}*/
		bone = new std::vector<Frame *>();

		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL) continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType == FbxNodeAttribute::eSkeleton)	// Transform 만들기
			{
				LoadBoneHierarachy(pFbxChildNode, *bone);
				// Animation 생성
				// Frame 생성
				// Animation curve 생성
				// Animation에 animation curve, frame 추가
			}
			else if (AttributeType == FbxNodeAttribute::eMesh)
			{
				int deformerCount = pFbxChildNode->GetMesh()->GetDeformerCount();
				
				// deformerCount == 0이면 Static Mesh라는 것.
				frame[j] = new Frame();
				frame[j]->Load(*pFbxChildNode, nullptr);

				mesh[j] = new SkinnedMesh(*g_pd3dDevice);
				mesh[j]->Load(*pFbxChildNode, bone);
				
				// defomerCount > 0 면 Skinned Mesh를 생성
				j++;
			}			
		}
	}

	pFbxRootNode->Destroy();
	scene->Destroy();
	fbxManager->Destroy();	

	return true;
}



VOID Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(32, 64, 128), 1.0f, 0);


	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		
		for (auto it = bone->begin(); it != bone->end(); ++it)
		{
			(*it)->Draw(*g_pd3dDevice);
		}

		for (int i = 0; i < 1; i++)
		{
			mesh[i]->ApplyMatrix(&frame[i]->GetWorldMatrixParent(), &frame[i]->GetLocalMatrix());
			//g_pd3dDevice->SetTransform(D3DTS_WORLD, &frame[i]->GetWorldMatrix());
			mesh[i]->Draw();
		}
			
		

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




VOID Cleanup()
{
	for(auto it = bone->begin(); it != bone->end(); ++it)
	{
		SAFE_DELETE(*it);		
	}
	bone->clear();
	SAFE_DELETE(bone);


	for (int i = 0; i < 6; i++)
	{
		SAFE_DELETE(mesh[i]);
		SAFE_DELETE(frame[i]);
	}

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
		// Create the vertex buffer
		if (SUCCEEDED(fbxSdk()))
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
					PostMessage(hWnd, WM_QUIT, 0, 0);
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
		Cleanup();
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}