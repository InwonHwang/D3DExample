#include <Windows.h>
#include <tchar.h>
#include <fbxsdk.h>
#include <cassert>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Description.h"

bool fbxSdk();

namespace std { typedef basic_string<TCHAR> tstring; }

void DebugBox(HRESULT hr, LPCTSTR str)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%i"), hr);

	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
}

std::tstring floatToString(float f)
{
	TCHAR szBuffer[50];
	_stprintf_s(szBuffer, _T("%f"), f);

	return std::tstring(szBuffer);
}
LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
VertexBuffer			VB[5];
IndexBuffer				IB[5];


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
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 eye(0, 10, -10);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 lookAt(0,0,0);
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &eye, &lookAt, &up);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), 1.0f, 1.0f, 10000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


VOID Cleanup()
{	
	VB.Destroy();
	IB.Destroy();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

VOID Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);


	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		LPDIRECT3DVERTEXBUFFER9 vertexTemp = static_cast<LPDIRECT3DVERTEXBUFFER9>(VB.GetData());
		LPDIRECT3DINDEXBUFFER9 indexTemp = static_cast<LPDIRECT3DINDEXBUFFER9>(IB.GetData());
		g_pd3dDevice->SetStreamSource(0, vertexTemp, 0, sizeof(MeshVertex));
		g_pd3dDevice->SetFVF(MeshVertex::FVF);
		g_pd3dDevice->SetIndices(indexTemp);
		//g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 1440, 0, 360);
		//g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 94);
		

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
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

	const char* fileName = "Media\\lowpolytree.fbx";

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



	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;
			
			
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			DebugBox(pMesh->GetPolygonVertexCount(), _T("??"));
			/*if (VB.Create(*g_pd3dDevice, pMesh->GetPolygonVertexCount(), sizeof(MESHVERTEX), 0, MESHVERTEX::FVF, D3DPOOL_DEFAULT))
			{
				VB.Load(*pFbxChildNode);
			}*/

			DebugBox(pMesh->GetPolygonCount(), _T(""));
			/*if (IB.Create(*g_pd3dDevice, pMesh->GetPolygonCount(), sizeof(INDEX), 0, D3DPOOL_DEFAULT))
			{
				IB.Load(*pFbxChildNode);
			}*/

			
			//break;
		}
	}

	pFbxRootNode->Destroy();
	scene->Destroy();
	fbxManager->Destroy();	

	return true;
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