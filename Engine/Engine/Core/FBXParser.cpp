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
			// ���� ����.
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
	int vertexCount = 0;	// index������ ��.
	int primitiveCount = 0;

	Vector3 position;
	Vector4 color;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;

	for (int i = 0; i < polygonCount; ++i)
	{
		// polygon �ȿ� ���ؽ��� � �ִ���. ���� 3��, 4���� ��쵵 ����. triangulate �Լ��� ���� ��� 3���� ����
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

	// index �� �б�
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
	
	// Local Matrix �� �б�
	meshData->_matLocal = FbxDXUtil::ToDXMatrix(fbxNode.EvaluateLocalTransform());
	
	// �θ��� Matrix �� ���
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
					currJointIndex = i;	// �ε���
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

			double *pWeights = currCluster->GetControlPointWeights();		// �ش� ���� ���� ������ ����ġ
			int *pIndices = currCluster->GetControlPointIndices();			// �ش� ���� ������ �޴� ������																				
			int numBoneVertexIndices = currCluster->GetControlPointIndicesCount(); // �ش� ���� ������ �޴� ��� ������ �ϳ��� ������	

			for (int k = 0; k < numBoneVertexIndices; k++)
			{
				float tempWeight = (float)pWeights[k];	// ������ �޴� ������ ����ġ ����
				int tempIndex = pIndices[k];			// ������ �޴� ������ �ε���

														// ����ġ�� 0�̸� �����ɷ� �ѱ�
				if (tempWeight == 0) continue;

				SKININFO skinInfo;
				// ����ġ �� x�� 0�̸� ù��° �ε���
				//if (bufferVertex[tempIndex].Weight.x == 0)
				//{
				//	bufferVertex[tempIndex].Weight.x = tempWeight;
				//	bufferVertex[tempIndex].Index[0] = currJointIndex;
				//}
				//// ����ġ �� x�� 0�� �ƴϰ� y�� 0�̸� �ι�° �ε���
				//else if (bufferVertex[tempIndex].Weight.x != 0 &&
				//	bufferVertex[tempIndex].Weight.y == 0)
				//{
				//	bufferVertex[tempIndex].Weight.y = tempWeight;
				//	bufferVertex[tempIndex].Index[1] = currJointIndex;
				//}
				//// ����ġ �� x�� 0�� �ƴϰ� y�� 0�� �ƴϰ� z�� 0�̸� ����° �ε���
				//else if (bufferVertex[tempIndex].Weight.x != 0 &&
				//	bufferVertex[tempIndex].Weight.y != 0 &&
				//	bufferVertex[tempIndex].Weight.z == 0)
				//{
				//	bufferVertex[tempIndex].Weight.z = tempWeight;
				//	bufferVertex[tempIndex].Index[2] = currJointIndex;
				//}
				//// ��� 0�� �ƴϸ� 4��° �ε���, ����ġ�� 1���� xyz ���� ������ ���� �� ����
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