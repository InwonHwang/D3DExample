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
	FbxGeometryConverter converter(&fbxManager);
	converter.Triangulate(_scene, true);

	// ��ǥ�� ����
	/*FbxAxisSystem axisSystem = FbxAxisSystem::DirectX;

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
	ReadMatrix(fbxNode, transformData->local, transformData->world);

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
	
	// ��Ʈ���� �б�
	ReadMatrix(fbxNode, boneData->local, boneData->world);

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

	// ��� transform ���ϱ�
	FbxVector4 lT = fbxNode.GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 lR = fbxNode.GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 lS = fbxNode.GetGeometricScaling(FbxNode::eSourcePivot);

	FbxAMatrix transform = FbxAMatrix(lT, lR, lS);
	transform.SetS(FbxVector4(1.0f, 1.0f, 1.0f));

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
		FbxSkin* pSkin = reinterpret_cast< FbxSkin* >(mesh->GetDeformer(i, FbxDeformer::eSkin));

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

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;// *transform;
			//FbxVector4 vS = globalBindposeInverseMatrix.GetS();
			//globalBindposeInverseMatrix.SetS(FbxVector4(vS.mData[0], -vS.mData[1], vS.mData[2], vS.mData[3]));

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

				meshData->verticeDataVec[tempIndex].vertexBlendingInfoVec.push_back(bi);
			}

			// Animation ����
			
			FbxAnimStack* animStack = _scene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = animStack->GetName();
			
			FbxTakeInfo* takeInfo = _scene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			
			boneDataVec[curBoneIdx]->start = static_cast<int>(start.GetFrameCount(FbxTime::eFrames24));
			boneDataVec[curBoneIdx]->end = static_cast<int>(end.GetFrameCount(FbxTime::eFrames24));
			KEYFRAME** currAnim = &boneDataVec[curBoneIdx]->pAnimation;

			for (FbxLongLong l = start.GetFrameCount(FbxTime::eFrames24); l <= end.GetFrameCount(FbxTime::eFrames24); ++l)
			{
				*currAnim = new KEYFRAME();

				FbxTime currTime;
				currTime.SetFrame(l, FbxTime::eFrames24);
				
				(*currAnim)->frame = l;								
				FbxAMatrix currentTransformOffset = fbxNode.EvaluateGlobalTransform(currTime);// *transform;
				(*currAnim)->globalTransform = currentTransformOffset.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(currTime);

				currAnim = &((*currAnim)->pNext);
			} // for l
		} // for j
	} // for i

	VERTEXBLENDINGINFO tempBlendInfo;
	tempBlendInfo.blendingIndex = 0;
	tempBlendInfo.blendingWeight = 0.0f;

	for (size_t i = 0; i < meshData->verticeDataVec.size(); ++i)
	{
		for (size_t j = meshData->verticeDataVec[i].vertexBlendingInfoVec.size(); j < 4; ++j)
		{
			meshData->verticeDataVec[i].vertexBlendingInfoVec.push_back(tempBlendInfo);
		}
		meshData->verticeDataVec[i].SortBlendingInfoByWeight();
	}
	

	for (auto data : boneDataVec)
		data.reset();

	boneDataVec.clear();
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

	// vertex, index ���� �б�
	for (int i = 0; i < polygonCount; ++i)
	{
		// polygon �ȿ� ���ؽ��� � �ִ���. ���� 3��, 4���� ��쵵 ����. triangulate �Լ��� ���� ��� 3���� ����
		int polygonSize = pMesh->GetPolygonSize(i);
		for (int j = 0; j < polygonSize; ++j)
		{
			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			

			ReadPosition(*pMesh, ctrlPointIndex, tempVertex.vertex.position);
			ReadColor(*pMesh, ctrlPointIndex, vertexCount, tempVertex.vertex.color);
			ReadNormal(*pMesh, ctrlPointIndex, vertexCount, tempVertex.vertex.normal);
			ReadUV(*pMesh, ctrlPointIndex, vertexCount, tempVertex.vertex.texCoord);
			ReadTangent(*pMesh, ctrlPointIndex, vertexCount, tempVertex.vertex.tangent);
			
			meshData->verticeDataVec.push_back(tempVertex); // vertex ��
			meshData->indiceDataVec.push_back(vertexCount); // index ��

			vertexCount++;
		}
	}

	// Skin���� �б�
	ReadSkinInfo(fbxNode, fbxData, meshData);
	
	// Local Matrix �� �б�
	ReadMatrix(fbxNode, meshData->local, meshData->world);

	// �̸� �б�
	StringUtil::SetName(fbxNode.GetName(), meshData->name);

	fbxData.fbxDataVec.push_back(meshData);
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