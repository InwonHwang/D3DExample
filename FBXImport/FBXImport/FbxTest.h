#pragma once
#include "Core.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "Frame.h"

class FbxTest
{
public:
	FbxTest();
	~FbxTest();

	//static IDirect3DDevice9* g_pd3dDevice;

	static std::vector<StaticMesh *> g_staticMeshs;
	static std::vector<SkinnedMesh *> g_skinnedMeshs;
	static std::vector<Frame *> g_frames;
	static std::vector<Frame *> g_bones;
	static FbxManager* g_pFbxManager;
	static FbxScene* g_pScene;

	static bool Init();
	static bool ImportScene();
	static void Load();
	static void LoadNodeAll(FbxNode& node);

	static void Clear();
};

