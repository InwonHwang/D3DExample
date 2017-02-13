#include "FBXParser.h"

bool FBXMgr::Init()
{
	_fbxManager = FbxManager::Create();

	if (!_fbxManager) return false;

	return true;
}

void FBXMgr::Release()
{
	if (_fbxManager) _fbxManager->Destroy();
}

bool FBXParser::Init(FbxManager& fbxManager, const String& fileName)
{	
	FbxIOSettings* ios = FbxIOSettings::Create(&fbxManager, IOSROOT);

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

	fbxManager.SetIOSettings(ios);

	// scene 생성
	_scene = FbxScene::Create(&fbxManager, "");

	// importer 생성
	FbxImporter* pFbxImporter = FbxImporter::Create(&fbxManager, "");	

	// 현재 설정으로 해당 fbx임포트가 가능한지 확인
	bool importStatus = pFbxImporter->Initialize("Media\\Fbx\\Hero_General.fbx", -1, fbxManager.GetIOSettings());

	if (!importStatus)
	{
		return false;
	}

	// 임포트
	_scene->Clear();	
	importStatus = pFbxImporter->Import(_scene);
	pFbxImporter->Destroy();

	// triangle방식으로 변경 (ctrl point 방식 X)
	FbxGeometryConverter converter(&fbxManager);
	converter.Triangulate(_scene, true);

	// 좌표계 설정
	FbxAxisSystem axisSystem = FbxAxisSystem::DirectX;

	if (_scene->GetGlobalSettings().GetAxisSystem() != axisSystem)
	{
		axisSystem.ConvertScene(_scene);
	}

	return importStatus;
}
void FBXParser::Release()
{
	if (_scene) _scene->Destroy();
}

void FBXParser::Load(FBXData& fbxData)
{
	assert(_scene && "null reference: _scene");
	FbxNode* pRootNode = _scene->GetRootNode();

	if (pRootNode)
		LoadRecursively(*pRootNode, fbxData);
}

void FBXParser::LoadRecursively(FbxNode& fbxNode, FBXData& fbxData)
{
	if (fbxNode.GetNodeAttribute())
	{
		FbxNodeAttribute::EType attType = fbxNode.GetNodeAttribute()->GetAttributeType();
		switch (attType)
		{
			// 지원 안함.
			case FbxNodeAttribute::eMarker:
				break;
							
			case FbxNodeAttribute::eSkeleton:
				break;

			case FbxNodeAttribute::eMesh:
			{				
				ReadMesh(fbxNode, fbxData);				
			}
				break;

			case FbxNodeAttribute::eNurbs:				
				break;

			case FbxNodeAttribute::ePatch:
				break;

			case FbxNodeAttribute::eCamera:				
				break;

			case FbxNodeAttribute::eLight:				
				break;

			case FbxNodeAttribute::eNull:				
				break;

			default:				
				break;
		}
	}

	for (int i = 0; i < fbxNode.GetChildCount(); ++i)
	{
		FbxNode* childNode = fbxNode.GetChild(i);
		LoadRecursively(*childNode, fbxData);
	}

}

void FBXParser::ReadMesh(FbxNode& fbxNode, FBXData& fbxData)
{
	FbxMesh* pMesh = fbxNode.GetMesh();

	assert(pMesh && "null reference: fbxMesh");

	sp<FBXMeshData> meshData(new FBXMeshData());
	fbxData._fbxMeshDataMgr._fbxMeshDataVec.push_back(meshData);
	
	std::vector<unsigned long>	bufferIndex;

	int polygonCount = pMesh->GetPolygonCount();
	int vertexCount = 0;	// index값으로 들어감.
	int primitiveCount = 0;

	Vector3 position;
	Vector4 color;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;

	for (int i = 0; i < polygonCount; ++i)
	{
		// polygon 안에 버텍스가 몇개 있는지. 보통 3개, 4개인 경우도 있음. triangulate 함수를 쓰면 모두 3개로 변경
		int polygonSize = pMesh->GetPolygonSize(i);
		for (int j = 0; j < polygonSize; ++j)
		{
			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			ReadPosition(*pMesh, ctrlPointIndex, position);
			ReadColor(*pMesh, ctrlPointIndex, vertexCount, color);
			ReadNormal(*pMesh, ctrlPointIndex, vertexCount, normal);
			ReadUV(*pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), texCoord);
			ReadTangent(*pMesh, ctrlPointIndex, vertexCount, tangent);

			meshData->_positionVec.push_back(position);
			meshData->_colorVec.push_back(color);
			meshData->_normalVec.push_back(normal);
			meshData->_texCroodVec.push_back(texCoord);
			meshData->_tangentVec.push_back(tangent);

			bufferIndex.push_back(vertexCount);

			vertexCount++;
		}
	}

	// index 값 읽기
	primitiveCount = bufferIndex.size() / 3;
	vertexCount = 0;
	INDEX index;

	for (int i = 0; i < primitiveCount; ++i)
	{		
		index._0 = bufferIndex[vertexCount++];
		index._1 = bufferIndex[vertexCount++];
		index._2 = bufferIndex[vertexCount++];		

		meshData->_indiceVec.push_back(index);
	}
	
	// Local Matrix 값 읽기
	meshData->_matLocal = FbxDXUtil::ToDXMatrix(fbxNode.EvaluateLocalTransform());
	
	// 부모의 Matrix 값 얻기
	FbxAMatrix matWorld;
	matWorld.SetIdentity();

	FbxNode* parent = fbxNode.GetParent();
	if (parent)
		matWorld = parent->EvaluateGlobalTransform();
	
	meshData->_matWorldParent = FbxDXUtil::ToDXMatrix(matWorld);
	
}

void FBXParser::ReadPosition(FbxMesh& mesh, int ctrlPointIndex, Vector3& position)
{
	FbxVector4* pCtrlPoint = mesh.GetControlPoints();

	position.x = (float)pCtrlPoint[ctrlPointIndex].mData[0];
	position.y = (float)pCtrlPoint[ctrlPointIndex].mData[1];
	position.z = (float)pCtrlPoint[ctrlPointIndex].mData[2];
}

void FBXParser::ReadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector4& color)
{
	if (mesh.GetElementVertexColorCount() < 1) return;

	FbxGeometryElementVertexColor* pVertexColor = mesh.GetElementVertexColor(0);

	FbxLayerElement::EMappingMode mappingMode = pVertexColor->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pVertexColor->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			color.x = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
			color.y = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
			color.z = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
			color.w = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
			color.x = (float)pVertexColor->GetDirectArray().GetAt(i).mRed;
			color.y = (float)pVertexColor->GetDirectArray().GetAt(i).mGreen;
			color.z = (float)pVertexColor->GetDirectArray().GetAt(i).mBlue;
			color.w = (float)pVertexColor->GetDirectArray().GetAt(i).mAlpha;
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			color.x = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mRed;
			color.y = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mGreen;
			color.z = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mBlue;
			color.w = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mAlpha;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pVertexColor->GetIndexArray().GetAt(vertexCount);
			color.x = (float)pVertexColor->GetDirectArray().GetAt(i).mRed;
			color.y = (float)pVertexColor->GetDirectArray().GetAt(i).mGreen;
			color.z = (float)pVertexColor->GetDirectArray().GetAt(i).mBlue;
			color.w = (float)pVertexColor->GetDirectArray().GetAt(i).mAlpha;
		}
		break;
		}
		break;
	}
}

void FBXParser::ReadUV(FbxMesh& mesh, int ctrlPointIndex, int uvIndex, Vector2& uv)
{
	if (mesh.GetElementUVCount() < 1) return;

	FbxGeometryElementUV* pUV = mesh.GetElementUV(0);

	FbxLayerElement::EMappingMode mappingMode = pUV->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pUV->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			uv.x = (float)pUV->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			uv.y = (float)pUV->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pUV->GetIndexArray().GetAt(ctrlPointIndex);
			uv.x = (float)pUV->GetDirectArray().GetAt(i).mData[0];
			uv.y = (float)pUV->GetDirectArray().GetAt(i).mData[1];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			uv.x = (float)pUV->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.y = (float)pUV->GetDirectArray().GetAt(uvIndex).mData[1];
		}
		break;
		}
		break;
	}
}

void FBXParser::ReadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector3& normal)
{
	if (mesh.GetElementNormalCount() < 1) return;

	FbxGeometryElementNormal* pNormal = mesh.GetElementNormal(0);

	FbxLayerElement::EMappingMode mappingMode = pNormal->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			normal.x = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			normal.y = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
			normal.z = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pNormal->GetIndexArray().GetAt(ctrlPointIndex);
			normal.x = (float)pNormal->GetDirectArray().GetAt(i).mData[0];
			normal.y = (float)pNormal->GetDirectArray().GetAt(i).mData[1];
			normal.z = (float)pNormal->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			normal.x = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[0];
			normal.y = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[1];
			normal.z = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pNormal->GetIndexArray().GetAt(vertexCount);
			normal.x = (float)pNormal->GetDirectArray().GetAt(i).mData[0];
			normal.y = (float)pNormal->GetDirectArray().GetAt(i).mData[1];
			normal.z = (float)pNormal->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}
		break;
	}
}

void FBXParser::ReadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector3& tangent)
{
	if (mesh.GetElementTangentCount() < 1) return;

	FbxGeometryElementTangent* pTangent = mesh.GetElementTangent(0);

	FbxLayerElement::EMappingMode mappingMode = pTangent->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pTangent->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			tangent.x = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			tangent.y = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
			tangent.z = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pTangent->GetIndexArray().GetAt(ctrlPointIndex);
			tangent.x = (float)pTangent->GetDirectArray().GetAt(i).mData[0];
			tangent.y = (float)pTangent->GetDirectArray().GetAt(i).mData[1];
			tangent.z = (float)pTangent->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			tangent.x = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[0];
			tangent.y = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[1];
			tangent.z = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pTangent->GetIndexArray().GetAt(vertexCount);
			tangent.x = (float)pTangent->GetDirectArray().GetAt(i).mData[0];
			tangent.y = (float)pTangent->GetDirectArray().GetAt(i).mData[1];
			tangent.z = (float)pTangent->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}
		break;
	}
}

void FBXParser::ReadSkinInfo(FbxMesh& mesh, sp<FBXMeshData> meshData)
{
	FbxMesh* currMesh = fbxNode.GetMesh();
	DWORD dwDeformerCnt = (DWORD)currMesh->GetDeformerCount();
	// This geometry transform is something I cannot understand
	// I think it is from MotionBuilder
	// If you are using Maya for your models, 99% this is just an
	// identity matrix
	// But I am taking it into account anyways......

	// A deformer is a FBX thing, which contains some clusters
	// A cluster contains a link, which is basically a joint
	// Normally, there is only one deformer in a mesh

	const FbxVector4 lT = fbxNode.GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = fbxNode.GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = fbxNode.GetGeometricScaling(FbxNode::eSourcePivot);

	FbxAMatrix t(lT, lR, lS);

	for (DWORD i = 0; i < dwDeformerCnt; ++i)
	{
		// There are many types of deformers in Maya,
		// We are using only skins, so we see if this is a skin
		FbxSkin* currSkin = reinterpret_cast< FbxSkin* >(currMesh->GetDeformer(i, FbxDeformer::eSkin));
		if (!currSkin)	continue;

		_boneOffset.resize(_bones->size());

		int boneOffsetCount = _boneOffset.size();
		D3DXMATRIX identity(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		D3DXMatrixIdentity(&identity);

		for (int i = 0; i < _boneOffset.size(); ++i)
		{
			_boneOffset[i] = identity;
		}


		DWORD dwClusterCnt = (DWORD)currSkin->GetClusterCount();

		for (DWORD j = 0; j < dwClusterCnt; ++j)
		{
			FbxCluster* currCluster = currSkin->GetCluster(j);
			std::string currJointName = currCluster->GetLink()->GetName();
			int currJointIndex = 0;



			for (int i = 0; i < _bones->size(); ++i)
			{
				if (currJointName.compare(_bones->data()[i]->GetName()) == 0)
				{
					currJointIndex = i;	// 인덱스
					break;
				}
			}

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * t;

			_boneOffset[currJointIndex] = FbxDXUtil::ToDXMatrix(globalBindposeInverseMatrix);

			double *pWeights = currCluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
			int *pIndices = currCluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들																				
			int numBoneVertexIndices = currCluster->GetControlPointIndicesCount(); // 해당 본에 영향을 받는 모든 정점을 하나씩 가져옴	

			for (int k = 0; k < numBoneVertexIndices; k++)
			{
				float tempWeight = (float)pWeights[k];	// 영향을 받는 정점의 가중치 정도
				int tempIndex = pIndices[k];			// 영향을 받는 정점의 인덱스

														// 가중치가 0이면 다음걸로 넘김
				if (tempWeight == 0) continue;

				SKININFO skinInfo;
				// 가중치 중 x가 0이면 첫번째 인덱스
				//if (bufferVertex[tempIndex].Weight.x == 0)
				//{
				//	bufferVertex[tempIndex].Weight.x = tempWeight;
				//	bufferVertex[tempIndex].Index[0] = currJointIndex;
				//}
				//// 가중치 중 x가 0이 아니고 y가 0이면 두번째 인덱스
				//else if (bufferVertex[tempIndex].Weight.x != 0 &&
				//	bufferVertex[tempIndex].Weight.y == 0)
				//{
				//	bufferVertex[tempIndex].Weight.y = tempWeight;
				//	bufferVertex[tempIndex].Index[1] = currJointIndex;
				//}
				//// 가중치 중 x가 0이 아니고 y가 0이 아니고 z가 0이면 세번째 인덱스
				//else if (bufferVertex[tempIndex].Weight.x != 0 &&
				//	bufferVertex[tempIndex].Weight.y != 0 &&
				//	bufferVertex[tempIndex].Weight.z == 0)
				//{
				//	bufferVertex[tempIndex].Weight.z = tempWeight;
				//	bufferVertex[tempIndex].Index[2] = currJointIndex;
				//}
				//// 모두 0이 아니면 4번째 인덱스, 가중치는 1에서 xyz 빼면 나머지 구할 수 있음
				//else if (bufferVertex[tempIndex].Weight.x != 0 &&
				//	bufferVertex[tempIndex].Weight.y != 0 &&
				//	bufferVertex[tempIndex].Weight.z != 0)
				//{
				//	bufferVertex[tempIndex].Index[3] = currJointIndex;
				//}
			} // k

		}
	}
}