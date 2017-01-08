#include "Core.h"
#include "StaticMesh.h"
#include "VertexDescription.h"

bool fbxSdk();


LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
StaticMesh*				Mesh[6];
D3DXMATRIX				matrix[6];

static D3DXVECTOR4 max_to_dx(const FbxDouble4 &m, bool neg_w) {
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (neg_w ? -1 : 1) * (float)m[3]);
}

static D3DXVECTOR4 max_to_dx(const FbxDouble4 &m) {
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

static D3DXVECTOR3 max_to_dx(const FbxDouble3 &m) {
	return D3DXVECTOR3((float)m[0], (float)m[1], (float)m[2]);
}


static D3DXMATRIX max_to_dx(const FbxAMatrix &mtx)
{
	D3DXMATRIX mtx_s, mtx_r, mtx_t;

	
	auto s = max_to_dx(mtx.GetS());
	auto q = max_to_dx(mtx.GetQ(), false);
	auto t = max_to_dx(mtx.GetT());
		
	return
		*D3DXMatrixScaling(&mtx_s, (float)s[0], (float)s[1], (float)s[2]) *
		*D3DXMatrixRotationQuaternion(&mtx_r, &D3DXQUATERNION((float)q[0], (float)q[1], (float)q[2], (float)q[3])) *
		*D3DXMatrixTranslation(&mtx_t, (float)t[0], (float)t[1], (float)t[2]);
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
	
	D3DXVECTOR3 eye(-0, 0, -50);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 lookAt(-0, 0, 0);


	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &eye, &lookAt, &up);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), 1.0f, 1.0f, 10000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

bool fbxSdk()
{
	// fbx ½Ì±ÛÅæ
	FbxManager* fbxManager = FbxManager::Create();

	// IO ¼¼ÆÃ
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

	// Scene ÀÓÆ÷Æ®
	FbxImporter* importer = FbxImporter::Create(fbxManager, "");

	//const char* fileName = "Media\\lowpolytree.fbx";
	//const char* fileName = "Media\\Medieva_fantasy_house.fbx";
	const char* fileName = "Media\\baum hd med fbx.fbx";

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

	/*FbxAxisSystem axisSystem(FbxAxisSystem::DirectX);	

	FbxAxisSystem sceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();

	if (sceneAxisSystem != axisSystem)
	{
		axisSystem.ConvertScene(scene);
	}*/

	int j = 0;
	if (pFbxRootNode)
	{
		FbxAMatrix w = pFbxRootNode->EvaluateGlobalTransform();

		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;
			

			if (j < 6)
			{
				DebugBox(0, 0);
				FbxAMatrix lm = pFbxChildNode->EvaluateLocalTransform();
				matrix[j] = max_to_dx(lm * w) ;

				
				Mesh[j] = new StaticMesh(*g_pd3dDevice);
				Mesh[j]->Load(*pFbxChildNode);
			}


			j++;
		}
	}

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
		
		for (int i = 0; i < 1; i++)
		{
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matrix[i]);
			Mesh[i]->Draw();
		}
			
		

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




VOID Cleanup()
{
	
	for (int i = 0; i < 6; i++)
	{
		SAFE_DELETE(Mesh[i]);
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
	
	

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the vertex buffer
		if (SUCCEEDED(fbxSdk()))
		{
			InitMatrix();
			while (true)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
						break;

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
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