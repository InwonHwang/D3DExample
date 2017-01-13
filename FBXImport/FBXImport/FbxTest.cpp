#include "FbxTest.h"

extern IDirect3DDevice9* g_pd3dDevice;

FbxManager* FbxTest::g_pFbxManager = nullptr;
FbxScene* FbxTest::g_pScene = nullptr;
std::vector<StaticMesh *> FbxTest::g_staticMeshs;
std::vector<SkinnedMesh *>  FbxTest::g_skinnedMeshs;
std::vector<Frame *>FbxTest::g_frames;
std::vector<Frame *>FbxTest::g_bones;

FbxTest::FbxTest()
{
}


FbxTest::~FbxTest()
{
}

bool FbxTest::Init()
{
	g_pFbxManager = FbxManager::Create();

	if (!g_pFbxManager) return false;

	// FbxManager 세팅, singleton, 멀티쓰레드 상에 동기화지원 안됨.
	FbxIOSettings* ios = FbxIOSettings::Create(g_pFbxManager, IOSROOT);

	// 임포트값 세팅
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	ios->SetBoolProp(IMP_FBX_LINK, true);
	ios->SetBoolProp(IMP_FBX_SHAPE, true);
	ios->SetBoolProp(IMP_FBX_GOBO, true);
	ios->SetBoolProp(IMP_FBX_ANIMATION, true);
	ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	
	// 익스포트값 세팅
	ios->SetBoolProp(EXP_FBX_MATERIAL, true);
	ios->SetBoolProp(EXP_FBX_TEXTURE, true);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	ios->SetBoolProp(EXP_FBX_SHAPE, true);
	ios->SetBoolProp(EXP_FBX_GOBO, true);
	ios->SetBoolProp(EXP_FBX_ANIMATION, true);
	ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);
		
	g_pFbxManager->SetIOSettings(ios);

	g_pScene = FbxScene::Create(g_pFbxManager, "");

	return true;
}

bool FbxTest::ImportScene()
{
	if (!g_pFbxManager) return false;
	
	FbxImporter* pFbxImporter = FbxImporter::Create(g_pFbxManager, "");
	//const char* fileName = "Media\\lowpolytree.fbx";
	//const char* fileName = "Media\\Medieva_fantasy_house.fbx";
	//const char* fileName = "Media\\baum hd med fbx.fbx";
	const char* fileName = "Media\\Hero_General.fbx";
	//const char* fileName = "Media\\robot_red_arm_left.fbx";

	bool importStatus = pFbxImporter->Initialize(fileName, -1, g_pFbxManager->GetIOSettings());

	if (!importStatus) return false;
	
	
	g_pScene->Clear();
	importStatus = pFbxImporter->Import(g_pScene);

	pFbxImporter->Destroy();

	/*FbxAxisSystem axisSystem = FbxAxisSystem::eDirectX;

	if (g_pScene->GetGlobalSettings().GetAxisSystem() != axisSystem)
	{
		axisSystem.ConvertScene(g_pScene);
	}*/

	return importStatus;	
}

void FbxTest::Load()
{
	FbxNode* pRootNode = g_pScene->GetRootNode();

	if (pRootNode) LoadNodeAll(*pRootNode);
}

void FbxTest::LoadNodeAll(FbxNode& node)
{
	if (node.GetNodeAttribute())
	{
		FbxNodeAttribute::EType attType = node.GetNodeAttribute()->GetAttributeType();
		switch (attType)
		{
		case FbxNodeAttribute::eMesh :
			{
				/*StaticMesh* smesh = new StaticMesh(*g_pd3dDevice);
				smesh->Load(node, nullptr);
				g_staticMeshs.push_back(smesh);*/
				if (node.GetMesh()->GetDeformerCount() > 0)
				{
					Frame* frame = new Frame();
					frame->Load(node, nullptr);
					g_frames.push_back(frame);

					SkinnedMesh* smesh = new SkinnedMesh(*g_pd3dDevice);
					smesh->Load(node, (void*)&g_bones);
					g_skinnedMeshs.push_back(smesh);
				}				
			}
			break;
		case FbxNodeAttribute::eSkeleton :
			{
				Frame* frame = new Frame();
				frame->Load(node, nullptr);
				g_bones.push_back(frame);
			}
			break;
		}
	}

	for (int i = 0; i < node.GetChildCount(); ++i)
	{
		LoadNodeAll(*node.GetChild(i));
	}
}

void FbxTest::Clear()
{
	for (auto it = g_bones.begin(); it != g_bones.end();)
	{
		SAFE_DELETE(*it);
		it = g_bones.erase(it);
	}
	g_bones.clear();

	for (auto it = g_frames.begin(); it != g_frames.end();)
	{
		SAFE_DELETE(*it);
		it = g_frames.erase(it);
	}
	g_frames.clear();

	for (auto it = g_staticMeshs.begin(); it != g_staticMeshs.end();)
	{
		SAFE_DELETE(*it);
		it = g_staticMeshs.erase(it);
	}
	g_staticMeshs.clear();

	for (auto it = g_skinnedMeshs.begin(); it != g_skinnedMeshs.end();)
	{
		SAFE_DELETE(*it);
		it = g_skinnedMeshs.erase(it);
	}
	g_skinnedMeshs.clear();

	g_pScene->Clear();
	if (g_pScene) g_pScene->Destroy();
	if (g_pFbxManager) g_pFbxManager->Destroy();
}