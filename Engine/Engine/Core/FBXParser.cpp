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

	// ����Ʈ�� ����
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	ios->SetBoolProp(IMP_FBX_LINK, true);
	ios->SetBoolProp(IMP_FBX_SHAPE, true);
	ios->SetBoolProp(IMP_FBX_GOBO, true);
	ios->SetBoolProp(IMP_FBX_ANIMATION, true);
	ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	// �ͽ���Ʈ�� ����
	ios->SetBoolProp(EXP_FBX_MATERIAL, true);
	ios->SetBoolProp(EXP_FBX_TEXTURE, true);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	ios->SetBoolProp(EXP_FBX_SHAPE, true);
	ios->SetBoolProp(EXP_FBX_GOBO, true);
	ios->SetBoolProp(EXP_FBX_ANIMATION, true);
	ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	fbxManager.SetIOSettings(ios);

	// scene ����
	_scene = FbxScene::Create(&fbxManager, "");

	// importer ����
	FbxImporter* pFbxImporter = FbxImporter::Create(&fbxManager, "");	

	// ���� �������� �ش� fbx����Ʈ�� �������� Ȯ��
	bool importStatus = pFbxImporter->Initialize("Media\\Fbx\\Hero_General.fbx", -1, fbxManager.GetIOSettings());

	if (!importStatus)
	{
		return false;
	}

	// ����Ʈ
	_scene->Clear();	
	importStatus = pFbxImporter->Import(_scene);
	pFbxImporter->Destroy();

	// triangle������� ���� (ctrl point ��� X)
	/*FbxGeometryConverter converter(&fbxManager);
	converter.Triangulate(_scene, true);

	// ��ǥ�� ����
	FbxAxisSystem axisSystem = FbxAxisSystem::DirectX;

	if (_scene->GetGlobalSettings().GetAxisSystem() != axisSystem)
	{
		axisSystem.ConvertScene(_scene);
	}*/

	return importStatus;
}
void FBXParser::Release()
{
	if (_scene) _scene->Destroy();
}

void FBXParser::Load(FBXDATASET& fbxData)
{
	assert(_scene && "null reference: _scene");
	FbxNode* pRootNode = _scene->GetRootNode();

	if (pRootNode)
		LoadRecursively(*pRootNode, fbxData, 0, -1);	
}

void FBXParser::LoadRecursively(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex)
{
	if (fbxNode.GetNodeAttribute())
	{
		FbxNodeAttribute::EType attType = fbxNode.GetNodeAttribute()->GetAttributeType();
		switch (attType)
		{
			// ���� ����.
			case FbxNodeAttribute::eMarker:
				break;
							
			case FbxNodeAttribute::eSkeleton:
				_existAnimation = true;
				ReadBone(fbxNode, fbxData, index, parentIndex);
				break;

			case FbxNodeAttribute::eMesh:					
				ReadMesh(fbxNode, fbxData, index, parentIndex);			
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
	else
	{
		// FBXTransformData �Ľ�
		ReadTransform(fbxNode, fbxData, index, parentIndex);
	}

	for (int i = 0; i < fbxNode.GetChildCount(); ++i)
	{
		FbxNode* childNode = fbxNode.GetChild(i);
		LoadRecursively(*childNode, fbxData, fbxData.fbxDataVec.size(), index);
	}

}

void FBXParser::ReadTransform(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex)
{	
	sp<FBXTRANSFORMDATA> transformData(new FBXTRANSFORMDATA);

	transformData->parentIndex = parentIndex;
	transformData->dataType = FBXDATA::eTransform;
	
	// ��Ʈ���� �б�
	ReadMatrix(fbxNode, transformData->local, transformData->worldParent);

	// �̸� �б�
	StringUtil::SetName(fbxNode.GetName(), transformData->name);
	
	fbxData.fbxDataVec.push_back(transformData);
}

void FBXParser::ReadMatrix(FbxNode& fbxNode, FbxAMatrix& local, FbxAMatrix& worldParent)
{
	// Local Matrix �� �б�
	local = fbxNode.EvaluateLocalTransform();

	// �θ��� Matrix �� ���
	FbxAMatrix matWorld;
	matWorld.SetIdentity();

	FbxNode* parent = fbxNode.GetParent();
	if (parent)
		matWorld = parent->EvaluateGlobalTransform();

	worldParent = matWorld;
}

void FBXParser::ReadBone(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex)
{
	sp<FBXBONEDATA> boneData(new FBXBONEDATA);
	boneData->parentIndex = parentIndex;
	boneData->dataType = FBXDATA::eBone;
	boneData->pNode = &fbxNode;
	
	// ��Ʈ���� �б�
	ReadMatrix(fbxNode, boneData->local, boneData->worldParent);

	// �̸� �б�
	StringUtil::SetName(fbxNode.GetName(), boneData->name);
	
	fbxData.fbxDataVec.push_back(boneData);
}

void FBXParser::ReadSkinInfo(FbxNode& fbxNode, FBXDATASET& fbxData, sp<FBXMESHDATA> meshData)
{
	FbxMesh* mesh = fbxNode.GetMesh();
	int deformerCount = mesh->GetDeformerCount();

	if (deformerCount < 1) return;

	meshData->isSkinned = true;

	std::vector<sp<FBXBONEDATA>> boneDataVec;

	// boneData�� �־�α�
	for (auto data : fbxData.fbxDataVec)
	{
		if (data->dataType == FBXDATA::eBone)
		{			
			sp<FBXBONEDATA> temp = boost::static_pointer_cast<FBXBONEDATA>(data);
			boneDataVec.push_back(temp);
		}
	}

	for (int i = 0; i < deformerCount; ++i)
	{
		FbxSkin* pSkin = FbxCast<FbxSkin>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (!pSkin)	continue;

		int clusterCount = pSkin->GetClusterCount();

		for (int j = 0; j < clusterCount; ++j)
		{
			FbxCluster* pCluster = pSkin->GetCluster(j);

			String name;		// ����� �� �̸�
			int curBoneIdx = 0; // ����� �� �ε���
			StringUtil::SetName(pCluster->GetLink()->GetName(), name);

			//����� ���� �ε��� ã��
			for (uint k = 0; k < boneDataVec.size(); ++k)
			{
				if (boneDataVec[k]->name.compare(name) == 0)
				{
					curBoneIdx = k;
					break;
				}
			}

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			pCluster->GetTransformMatrix(transformMatrix);			// The transformation of the mesh at binding time
			pCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;

			boneDataVec[curBoneIdx]->globalBindposeInverse = globalBindposeInverseMatrix;
			
			double *pWeights = pCluster->GetControlPointWeights();		// �ش� ���� ���� ������ ����ġ
			int *pIndices = pCluster->GetControlPointIndices();			// �ش� ���� ������ �޴� ������																
			
			int numBoneVertexIndices = pCluster->GetControlPointIndicesCount(); 
			
			for (int k = 0; k < numBoneVertexIndices; k++)
			{
				float tempWeight = (float)pWeights[k];	// ������ �޴� ������ ����ġ ����
				int tempIndex = pIndices[k];			// ������ �޴� ������ �ε���

				VERTEXBLENDINGINFO bi;
				bi.blendingIndex = curBoneIdx;
				bi.blendingWeight = tempWeight;
								
				meshData->controlPointDataVec[tempIndex].vertexBlendingInfoVec.push_back(bi);
			}

		} // for j
	} // for i
	
	ReadAnimation(fbxNode, boneDataVec);

	// ��Ű�� ���� 4���� ���߱�
	VERTEXBLENDINGINFO tempBlendInfo;
	tempBlendInfo.blendingIndex = 0;
	tempBlendInfo.blendingWeight = 0.0f;
	for (size_t i = 0; i < meshData->controlPointDataVec.size(); ++i)
	{
		for (size_t j = meshData->controlPointDataVec[i].vertexBlendingInfoVec.size(); j < 4; ++j)
		{			
			meshData->controlPointDataVec[i].vertexBlendingInfoVec.push_back(tempBlendInfo);			
		}
	}

	for (auto data : boneDataVec)
		data.reset();

	boneDataVec.clear();
}

void FBXParser::ReadAnimation(FbxNode& fbxNode, std::vector<sp<FBXBONEDATA>>& boneDataVec)
{
	FbxAnimStack* pAnimStack = _scene->GetSrcObject<FbxAnimStack>(0);
	FbxString animStackName = pAnimStack->GetName();

	FbxAMatrix transform;
	GetGeometryMatrix(fbxNode, transform);

	FbxTakeInfo* takeInfo = _scene->GetTakeInfo(animStackName);
	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

	for (uint i = 0; i < boneDataVec.size(); ++i)
	{
		boneDataVec[i]->start = static_cast<int>(start.GetFrameCount(FbxTime::eFrames24));
		boneDataVec[i]->end = static_cast<int>(end.GetFrameCount(FbxTime::eFrames24));

		for (FbxLongLong l = start.GetFrameCount(FbxTime::eFrames24); l <= end.GetFrameCount(FbxTime::eFrames24); ++l)
		{
			KEYFRAME* pAnim = new KEYFRAME();

			FbxTime currTime;
			currTime.SetFrame(l, FbxTime::eFrames24);
			pAnim->frame = l;

			pAnim->globalTransform = boneDataVec[i]->pNode->EvaluateGlobalTransform(currTime) * boneDataVec[i]->globalBindposeInverse;
			boneDataVec[i]->animVec.push_back(sp<KEYFRAME>(pAnim));
		} // for l
	}
}


void FBXParser::ReadMesh(FbxNode& fbxNode, FBXDATASET& fbxData, int index, int parentIndex)
{
	FbxMesh* pMesh = fbxNode.GetMesh();

	assert(pMesh && "null reference: fbxMesh");

	sp<FBXMESHDATA> meshData(new FBXMESHDATA());
	
	meshData->parentIndex = parentIndex;
	meshData->dataType = FBXDATA::eMesh;
	meshData->isSkinned = false;

	int polygonCount = pMesh->GetPolygonCount(); // ������ ����
	int vertexCount = 0;	// index������ ��.
	int primitiveCount = 0; // �ﰢ�� ����

	SKINNEDVERTEX tempVertex;

	// Control Point ������� �б�
	// ���ؽ� ��Ű�� ������ Control Point������θ� �о���� �� �־ Control Point������� �о���� ��
	// Triangle ������� ���� �ؾ���
	int ctrlPointCount = pMesh->GetControlPointsCount();
	for (int i = 0; i < ctrlPointCount; ++i)
	{
		ReadPositionControlPoint(*pMesh, i, tempVertex.vertex.position);
		meshData->controlPointDataVec.push_back(tempVertex);
	}

	// Skin���� �б�
	ReadSkinInfo(fbxNode, fbxData, meshData);

	// Color, Normal, UV, Tangent �� �б�
	for (int i = 0; i < polygonCount; ++i)
	{
		// polygon �ȿ� ���ؽ��� � �ִ���. ���� 3��, 4���� ��쵵 ����. triangulate �Լ��� ���� ��� 3���� ����
		int polygonSize = pMesh->GetPolygonSize(i);
		for (int j = 0; j < polygonSize; ++j)
		{
			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			ReadColor(*pMesh, ctrlPointIndex, vertexCount, meshData->controlPointDataVec[ctrlPointIndex].vertex.color);
			ReadNormal(*pMesh, ctrlPointIndex, vertexCount, meshData->controlPointDataVec[ctrlPointIndex].vertex.normal);
			ReadUV(*pMesh, ctrlPointIndex, vertexCount, meshData->controlPointDataVec[ctrlPointIndex].vertex.texCoord);
			ReadTangent(*pMesh, ctrlPointIndex, vertexCount, meshData->controlPointDataVec[ctrlPointIndex].vertex.tangent);
			
			vertexCount++;
		}
	}

	vertexCount = 0;
	for (int i = 0; i < polygonCount; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			tempVertex = meshData->controlPointDataVec[ctrlPointIndex];
			tempVertex.SortBlendingInfoByWeight();

			meshData->verticeDataVec.push_back(tempVertex); // vertex ��
			//meshData->indiceDataVec.push_back(vertexCount); // index ��

			vertexCount++;
		}
	}

	//reverse(meshData->verticeDataVec.begin(), meshData->verticeDataVec.end());
	
	// Local Matrix �� �б�
	ReadMatrix(fbxNode, meshData->local, meshData->worldParent);

	// �̸� �б�
	StringUtil::SetName(fbxNode.GetName(), meshData->name);

	Optimize(meshData);

	fbxData.fbxDataVec.push_back(meshData);
}

void FBXParser::ReadPositionControlPoint(FbxMesh& mesh, int ctrlPointCount, Vector3& position)
{
	position.x = static_cast<float>(mesh.GetControlPointAt(ctrlPointCount).mData[0]);
	if (_existAnimation)
	{
		position.y = static_cast< float >(mesh.GetControlPointAt(ctrlPointCount).mData[1]);
		position.z = static_cast< float >(mesh.GetControlPointAt(ctrlPointCount).mData[2]);
	}
	else
	{
		position.y = static_cast< float >(mesh.GetControlPointAt(ctrlPointCount).mData[2]);
		position.z = static_cast< float >(mesh.GetControlPointAt(ctrlPointCount).mData[1]);
	}
}

void FBXParser::ReadlPositionTriangle(FbxMesh& mesh, int ctrlPointIndex, Vector3& position)
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

void FBXParser::ReadUV(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, Vector2& uv)
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
			uv.x = (float)pUV->GetDirectArray().GetAt(vertexCount).mData[0];
			uv.y = (float)pUV->GetDirectArray().GetAt(vertexCount).mData[1];
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pUV->GetIndexArray().GetAt(vertexCount);
			uv.x = (float)pUV->GetDirectArray().GetAt(index).mData[0];
			uv.y = 1.0f - (float)pUV->GetDirectArray().GetAt(index).mData[1];
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
void FBXParser::Optimize(sp<FBXMESHDATA> meshData)
{
	
	std::vector<SKINNEDVERTEX> optVec;
	bool exist = false;

	DWORD j = 0;
	for (DWORD i = 0; i < meshData->verticeDataVec.size(); ++i)
	{
		for (j = 0; j < optVec.size(); ++j)
		{
			if (meshData->verticeDataVec[i].vertex == optVec[j].vertex/* &&
				meshData->verticeDataVec[i].vertexBlendingInfoVec.empty() == false &&
				meshData->verticeDataVec[i].vertexBlendingInfoVec[0] == optVec[j].vertexBlendingInfoVec[0] && 
				meshData->verticeDataVec[i].vertexBlendingInfoVec[1] == optVec[j].vertexBlendingInfoVec[1] &&
				meshData->verticeDataVec[i].vertexBlendingInfoVec[2] == optVec[j].vertexBlendingInfoVec[2] &&
				meshData->verticeDataVec[i].vertexBlendingInfoVec[3] == optVec[j].vertexBlendingInfoVec[3]*/)
			{	
				exist = true;
				break;
			}
		}

		if (!exist)
		{
			optVec.push_back(meshData->verticeDataVec[i]);
		}
		
		meshData->indiceDataVec.push_back(j);

		j = 0;
		exist = false;
	}

	meshData->verticeDataVec.swap(optVec);
}

void FBXParser::GetGeometryMatrix(FbxNode& fbxNode, FbxAMatrix& matrix)
{
	FbxVector4 lT = fbxNode.GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 lR = fbxNode.GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 lS = fbxNode.GetGeometricScaling(FbxNode::eSourcePivot);

	matrix = FbxAMatrix(lT, lR, lS);
	matrix.SetS(FbxVector4(1.0f, -1.0f, 1.0f));
}