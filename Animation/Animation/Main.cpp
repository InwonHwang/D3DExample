// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <list>

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define SAFE_RELEASE(p){if(p) {(p)->Release(); (p)=NULL;}}
#define SAFE_DELETE(p) {if(p) {delete(p); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p)=NULL;}}

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DSURFACE9 z_buffer = NULL;    // the pointer to the z-buffer

struct CUSTOM_FRAME : public D3DXFRAME
{
	D3DXMATRIX CombTransformationMatrix;    // transform data multiplied by all parents' data
};

struct CUSTOM_MESHCONTAINER : public D3DXMESHCONTAINER
{	
	D3DXMATRIX** ppFrameMatrices;    // an array of matrices to store current animation data
	LPD3DXMESH pFinalMesh;    // a duplicate of the mesh, representing it's current pose
	LPDIRECT3DTEXTURE9* pTextures;    // an array of applicable textures
};

class Transform
{
private:
	std::list<Transform *> _children;
	Transform * _parent;
	

public:
	D3DXFRAME* _frame;

	void SetChild(Transform * child)
	{
		_children.push_back(child);
	}

	void SetParent(Transform * parent)
	{
		_parent = parent;
	}

	void CreateFrame()
	{
		_frame = new CUSTOM_FRAME();
	}

	Transform() {}
	~Transform() { _children.clear(); _parent = NULL;}
};

std::list<Transform *> Transforms;
Transform* TopTransform;

// mesh declarations
LPD3DXMESH meshSpaceship;    // define the mesh pointer
D3DMATERIAL9* material;    // define the material object
LPDIRECT3DTEXTURE9* texture;    // a pointer to a texture
DWORD numMaterials;    // stores the number of materials in the mesh
//LPD3DXFRAME TopFrame = NULL;    // the top-most frame of the model
LPD3DXANIMATIONCONTROLLER AnimationController = NULL;    // an object that handles animation
D3DXMATRIX* FinalMatrices = NULL;    // a buffer for the matrices in their final state
int MaxFrames;    // the highest number of frames a mesh container can be affected by

				  // function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations
void init_light(void);    // sets up the light and the material
void link_frames(CUSTOM_FRAME* pFrame);
void update_frames(CUSTOM_FRAME* pFrame, D3DXMATRIX* pParentMatrix);
void update_mesh_containers(CUSTOM_FRAME* pFrame);
void draw_mesh(CUSTOM_FRAME* pFrame);

class MeshAllocation : public ID3DXAllocateHierarchy
{
	//STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
	//{
	//	// Create and initialize the custom frame struct
	//	CUSTOM_FRAME* pFrame = new CUSTOM_FRAME;
	//	*ppNewFrame = pFrame;
	//	ZeroMemory(pFrame, sizeof(CUSTOM_FRAME));

	//	// Copy the name
	//	if (Name)
	//	{
	//		UINT len = strlen(Name) + 1;
	//		pFrame->Name = new char[len];
	//		memcpy(pFrame->Name, Name, len);
	//	}

	//	return S_OK;
	//}

	STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
	{
		//GameObject gameobject = new GameObject();
		//gameobject->AddComponent<Transform>();
		//*ppNewFrame = gameobject->GetComponent<Transform>()->GetFrame();
		Transform* transform = new Transform();
		Transforms.push_back(transform);
		transform->CreateFrame();
		*ppNewFrame = transform->_frame;
		ZeroMemory(transform->_frame, sizeof(CUSTOM_FRAME));

		if (Name)
		{
			UINT len = strlen(Name) + 1;
			transform->_frame->Name = new char[len];
			memcpy(transform->_frame->Name, Name, len);
		}

		return S_OK;		
	}

	STDMETHOD(CreateMeshContainer)(LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer)
	{
		// Create and initialize the custom mesh container
		struct CUSTOM_MESHCONTAINER *pMeshContainer = new CUSTOM_MESHCONTAINER;
		*ppNewMeshContainer = pMeshContainer;
		ZeroMemory(pMeshContainer, sizeof(CUSTOM_MESHCONTAINER));

		// Check to make sure its a normal mesh
		if (pMeshData->Type != D3DXMESHTYPE_MESH)
		{
			DestroyMeshContainer(pMeshContainer);
			return E_FAIL;
		}

		// Copy the name
		if (Name)
		{
			UINT len = strlen(Name) + 1;
			pMeshContainer->Name = new char[len];
			memcpy(pMeshContainer->Name, Name, len);
		}

		// Copy the mesh data
		pMeshContainer->MeshData.Type = pMeshData->Type;
		pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
		pMeshContainer->MeshData.pMesh->AddRef();

		// Copy the materials
		pMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
		for (DWORD i = 0; i < NumMaterials; i++)
		{
			pMeshContainer->pMaterials[i] = pMaterials[i];
			pMeshContainer->pMaterials[i].MatD3D.Ambient = pMaterials[i].MatD3D.Diffuse;
		}

		// Don't bother with the effect instance
		pMeshContainer->pEffects = NULL;

		// Copy the number of materials
		pMeshContainer->NumMaterials = max(1, NumMaterials);

		// Copy the adjacency data
		pMeshContainer->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
		memcpy(pMeshContainer->pAdjacency,
			pAdjacency,
			sizeof(DWORD) * pMeshData->pMesh->GetNumFaces() * 3);

		// Only do these steps if there is skin info
		if (pSkinInfo)
		{
			// Copy the skin data
			pMeshContainer->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			// Allocate memory for the pointers to the frame matrices
			pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[pSkinInfo->GetNumBones()];
			for (DWORD i = 0; i < pSkinInfo->GetNumBones(); i++)
			{
				pMeshContainer->ppFrameMatrices[i] = NULL;
			}
		}

		// Create a duplicate of the mesh
		pMeshContainer->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED,
			NULL,
			d3ddev,
			&pMeshContainer->pFinalMesh);

		// load the textures for each material (if any)
		pMeshContainer->pTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
		for (DWORD i = 0; i < NumMaterials; i++)
		{
			pMeshContainer->pTextures[i] = 0;
			if (pMaterials[i].pTextureFilename)
			{
				D3DXCreateTextureFromFileA(d3ddev,
					pMeshContainer->pMaterials[i].pTextureFilename,
					&pMeshContainer->pTextures[i]);
			}
		}

		// Find the maximum number of frames
		if (pMeshContainer->pSkinInfo)
			MaxFrames = max(MaxFrames, (int)pMeshContainer->pSkinInfo->GetNumBones());

		return S_OK;
	}

	STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree)
	{
		// free the name and the frame
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);

		return S_OK;
	}

	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerToFree)
	{
		// cast the pointer to a CUSTOM_MESHCONTAINER
		CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pMeshContainerToFree;

		// free or release all the allocated memory
		SAFE_DELETE_ARRAY(pMeshContainer->Name);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pFinalMesh);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
		SAFE_RELEASE(pMeshContainer->pSkinInfo);
		SAFE_DELETE_ARRAY(pMeshContainer->ppFrameMatrices);
		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
			SAFE_RELEASE(pMeshContainer->pTextures[i]);
		SAFE_DELETE(pMeshContainer);

		return S_OK;
	}
};

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
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",
		L"Our Direct3D Program",
		WS_OVERLAPPEDWINDOW,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);	

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
	d3dpp.EnableAutoDepthStencil = TRUE;    // automatically run the z-buffer for us
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;    // 16-bit pixel format for the z-buffer

												  // create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	TopTransform = new Transform();
	Transforms.push_back(TopTransform);

	

	init_graphics();    // call the function to initialize the triangle	
	init_light();    // call the function to initialize the light and material

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));    // ambient light

	return;
}


// this is the function used to render a single frame
void render_frame(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();

	// SET UP THE TRANSFORMS

	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(1000.0f, 100.0f, 0.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,    // aspect ratio
		1.0f,    // the near view-plane
		2000.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	static float index = 0.0f; index += 0.03f;    // an ever-increasing float value
	D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
	D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matRotateY);    // set the world transform

													   // if the mesh is animated...
	if (AnimationController)
	{
		static DWORD Time = GetTickCount();

		// move the animation forward by the elapsed time
		AnimationController->AdvanceTime((GetTickCount() - Time) * 0.001f, NULL);

		// reset Time for the next time through
		Time = GetTickCount();
	}

	// update each combined matrix
	update_frames((CUSTOM_FRAME*)TopTransform->_frame, NULL);

	// update the mesh containers
	update_mesh_containers((CUSTOM_FRAME*)TopTransform->_frame);

	// render each mesh container
	draw_mesh((CUSTOM_FRAME*)TopTransform->_frame);

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	MeshAllocation MeshAllocator;

	SAFE_RELEASE(AnimationController);    // release the animation controller
	//D3DXFrameDestroy(TopFrame, &MeshAllocator);    // free all memory for the animated mesh
	D3DXFrameDestroy(TopTransform->_frame, &MeshAllocator);    // free all memory for the animated mesh
	
	for (std::list<Transform *>::iterator it = Transforms.begin(); it != Transforms.end(); it++)
	{
		SAFE_DELETE((*it));
	}

	Transforms.clear();
	SAFE_DELETE_ARRAY(FinalMatrices);    // free the FinalMatrices array
	SAFE_RELEASE(d3ddev);    // close and release the 3D device
	SAFE_RELEASE(d3d);    // close and release Direct3D
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	MeshAllocation MemAllocator;    // create a mesh allocation class



	D3DXLoadMeshHierarchyFromX(L"tiny.x",
		D3DXMESH_MANAGED,
		d3ddev,
		&MemAllocator,
		NULL,
		&TopTransform->_frame,
		&AnimationController);
	// allocate memory for the FinalMatrices array
	FinalMatrices = new D3DXMATRIX[MaxFrames];
	ZeroMemory(FinalMatrices, sizeof(D3DXMATRIX) * MaxFrames);

	// set up the ppFrameMatrices pointers
	link_frames((CUSTOM_FRAME*)TopTransform->_frame);
}


// this is the function that sets up the lights and materials
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

	d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
	d3ddev->LightEnable(0, TRUE);    // turn on light #0

	return;
}


void link_frames(CUSTOM_FRAME* pFrame)
{
	// cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;

	

	// if there is a mesh container and if it has skin info...
	if (pMeshContainer && pMeshContainer->pSkinInfo)
	{
		// loop through each frame in the mesh container
		for (UINT i = 0; i < pMeshContainer->pSkinInfo->GetNumBones(); i++)
		{
			CUSTOM_FRAME* pTempFrame;    // a temporary frame pointer

									 // find each frame by name

			pTempFrame = (CUSTOM_FRAME*)D3DXFrameFind(TopTransform->_frame,
				pMeshContainer->pSkinInfo->GetBoneName(i));
			// and set up a pointer to it

			
			pMeshContainer->ppFrameMatrices[i] = &pTempFrame->CombTransformationMatrix;
		}
	}

	// run for all siblings
	if (pFrame->pFrameSibling)
		link_frames((CUSTOM_FRAME*)pFrame->pFrameSibling);

	// run for the first child (which will then run all other children)
	if (pFrame->pFrameFirstChild)
		link_frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
}


void update_frames(CUSTOM_FRAME* pFrame, D3DXMATRIX* pParentMatrix) // frame Ã£±â
{
	// combine the frame's matrix with the parent's matrix, if any
	if (pParentMatrix)
		pFrame->CombTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	else
		pFrame->CombTransformationMatrix = pFrame->TransformationMatrix;

	// run for all siblings
	if (pFrame->pFrameSibling)
		update_frames((CUSTOM_FRAME*)pFrame->pFrameSibling, pParentMatrix);

	// run for the first child (which will then run all other children)
	if (pFrame->pFrameFirstChild)
		update_frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild,
			&pFrame->CombTransformationMatrix);
}

//int num = 0;
void update_mesh_containers(CUSTOM_FRAME* pFrame)
{
	//num++;;
	//char c[128];
	//sprintf(c, "%d", num);
	//MessageBoxA(GetActiveWindow(), c, NULL, MB_OK);
	// cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;

	
	if (pMeshContainer && pMeshContainer->pSkinInfo)
	{
		//if(pMeshContainer->Name)
			//MessageBoxA(GetActiveWindow(), pMeshContainer->Name, NULL, MB_OK);
		UINT NumFrames = pMeshContainer->pSkinInfo->GetNumBones();    // find how many frames
																	  // for each frame in the mesh container...
		for (UINT i = 0; i < NumFrames; i++)
		{
			// set FinalMatrices to that frame's offset matrix
			FinalMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

			// multiply that by the animated frame matrix
			FinalMatrices[i] *= *pMeshContainer->ppFrameMatrices[i];
		}

		void* pSrc = NULL;    // a void pointer for the original mesh
		void* pDst = NULL;    // a void pointer for the modified mesh

							  // lock the two meshes
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(NULL, &pSrc);
		pMeshContainer->pFinalMesh->LockVertexBuffer(NULL, &pDst);

		// store the animated mesh into FinalMesh
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(FinalMatrices, NULL, pSrc, pDst);

		// unlock the two meshes
		pMeshContainer->pFinalMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}

	// run for all siblings
	//if (pFrame->pFrameSibling)
		//update_mesh_containers((CUSTOM_FRAME*)pFrame->pFrameSibling);
//
	// run for the first child (which will then run all other children)
	if (pFrame->pFrameFirstChild)
		update_mesh_containers((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
}


void draw_mesh(CUSTOM_FRAME* pFrame)
{
	// cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;

	if (pMeshContainer)
	{
		// for each material...
		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			// set the material
			d3ddev->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			// set the texture
			d3ddev->SetTexture(0, pMeshContainer->pTextures[i]);

			// draw the subset!
			pMeshContainer->pFinalMesh->DrawSubset(i);
		}
	}


	if (pFrame->pFrameSibling)
		draw_mesh((CUSTOM_FRAME*)pFrame->pFrameSibling);

	// run for the first child (which will then run all other children)
	if (pFrame->pFrameFirstChild)
		draw_mesh((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
}
