#include "SkinnedMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Frame.h"
#include "FbxDXUtil.h"


SkinnedMesh::SkinnedMesh(IDirect3DDevice9& device)
	: Mesh(device)
{
	_isSkinned = true;
	_vb = new VertexBuffer(device);
	_ib = new IndexBuffer(device);
}


SkinnedMesh::~SkinnedMesh()
{	
	SAFE_DELETE(_vb);
	SAFE_DELETE(_ib);
}

void SkinnedMesh::ApplyMatrix(D3DXMATRIX& matLocal, D3DXMATRIX& matWorld)
{
	int idx = 0;
	
	D3DXMATRIX m;
	
	for (int i = 0; i < _indexBones.size(); i++)
	{
		idx = _indexBones[i];
		m = _boneOffset[idx];// *_bones->data()[idx]->GetWorldMatrix();
		//m *= matWorld;
		_device->SetTransform(D3DTS_WORLDMATRIX(idx), &m);
	}

	
}

void SkinnedMesh::Draw()
{
	assert(_vb->GetVertexBuffer() && "vertex buffer doesn't created!!");
	assert(_ib->GetData() && "index buffer doesn't created!!");
	
	_device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	_device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);
	
	_device->SetStreamSource(0, _vb->GetVertexBuffer(), 0, sizeof(SKINNEDMESHVERTEX));
	_device->SetVertexDeclaration(_vb->GetVertexDeclaration());
	_device->SetIndices(_ib->GetData());
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertexCount, 0, _primitiveCount);
	
	_device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	_device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
}

void SkinnedMesh::Load(FbxNode& fbxNode, void* pData)
{
	if (pData)
	{
		_bones = static_cast<std::vector<Frame *>*>(pData);
	}


	ProcessControlPoints(&fbxNode);
	ProcessJointsAndAnimations(&fbxNode);

	FbxMesh* pMesh = fbxNode.GetMesh();

	if (pMesh == nullptr) return;
	

	std::vector<SKINNEDMESHVERTEX>	bufferVertex;
	std::vector<unsigned long>		bufferIndex;

	int polygonCount = pMesh->GetPolygonCount();
	int vertexCount = 0;	// index값으로 들어감.

	for (int i = 0; i < polygonCount; ++i)
	{
		// polygon 안에 버텍스가 몇개 있는지. 보통 3개, 4개인 경우도 있음.
		int polygonSize = pMesh->GetPolygonSize(i);
		for (int j = 0; j < polygonSize; ++j)
		{
			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			SKINNEDMESHVERTEX vertex;

			LoadPosition(*pMesh, ctrlPointIndex, vertex.Position);
			LoadColor(*pMesh, ctrlPointIndex, vertexCount, vertex.Color);
			LoadNormal(*pMesh, ctrlPointIndex, vertexCount, vertex.Normal);
			LoadUV(*pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), vertex.TexCoord);
			LoadTangent(*pMesh, ctrlPointIndex, vertexCount, vertex.Tangent);
			mControlPoints[ctrlPointIndex]->SortBlendingInfoByWeight();
			
			vertex.Weight.x = mControlPoints[ctrlPointIndex]->mBlendingInfo[0].mBlendingWeight;
			vertex.Weight.y = mControlPoints[ctrlPointIndex]->mBlendingInfo[1].mBlendingWeight;
			vertex.Weight.z = mControlPoints[ctrlPointIndex]->mBlendingInfo[2].mBlendingWeight;
			vertex.Index[0] = mControlPoints[ctrlPointIndex]->mBlendingInfo[0].mBlendingIndex;
			vertex.Index[1] = mControlPoints[ctrlPointIndex]->mBlendingInfo[1].mBlendingIndex;
			vertex.Index[2] = mControlPoints[ctrlPointIndex]->mBlendingInfo[2].mBlendingIndex;
			vertex.Index[3] = mControlPoints[ctrlPointIndex]->mBlendingInfo[3].mBlendingIndex;

			bufferVertex.push_back(vertex);
			bufferIndex.push_back(vertexCount);

			vertexCount++;
		}
	}

	for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		delete itr->second;
	}
	mControlPoints.clear();

	_bones = static_cast<std::vector<Frame *>*>(pData);

	std::set<int> indexBone;
		
	for (int i = 0; i < bufferVertex.size(); i++) {
		for (int j = 0; j < 4; j++)	{
			indexBone.insert(bufferVertex[i].Index[j]);
		}					
	}
		
	for (auto it = indexBone.begin(); it != indexBone.end(); ++it) {
		_indexBones.push_back(*it);
	}	

	//정점 버퍼 정보 채우기
	_vertexCount = bufferVertex.size();
	int bufferVertexSize = _vertexCount * sizeof(SKINNEDMESHVERTEX);

	if (bufferVertexSize > 0)	// CreateVertexBuffer일 때 사이즈가 0이면 에러가 뜸.
	{
		_vb->Create(bufferVertexSize, 0, D3DPOOL_MANAGED);
		_vb->CreateVertexDeclaration(SkinnedMeshDec);

		void* pData;

		if (_vb->Lock(0, bufferVertexSize, &pData, 0))
		{
			memcpy(pData, &bufferVertex[0], bufferVertexSize);

			_vb->Unlock();
		}
	}

	// 인덱스 버퍼 내용 채우기
	_primitiveCount = bufferIndex.size() / 3;

	// 16bit 인덱스를 쓸건지 32bit 인덱스를 쓸건지 Device caps를 통해서 알아내기
	int bufferIndiceSize = bufferIndex.size() * sizeof(unsigned long);
	if (bufferIndiceSize > 0)
	{
		_ib->Create(bufferIndiceSize, 0, D3DPOOL_MANAGED);
		void * pData;
		if (_ib->Lock(0, bufferIndiceSize * sizeof(unsigned long), &pData, 0))
		{
			memcpy(pData, &bufferIndex[0], bufferIndiceSize);

			_ib->Unlock();
		}
	}
}
void SkinnedMesh::Save(FbxNode& fbxNode)
{

}


/////////////////////////////////////////////////////////////////////////////////////////////Test

void SkinnedMesh::ProcessControlPoints(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint();
		D3DXVECTOR3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
		currCtrlPoint->mPosition = currPosition;
		mControlPoints[i] = currCtrlPoint;
	}
}
void SkinnedMesh::ProcessJointsAndAnimations(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	// This geometry transform is something I cannot understand
	// I think it is from MotionBuilder
	// If you are using Maya for your models, 99% this is just an
	// identity matrix
	// But I am taking it into account anyways......
	FbxAMatrix geometryTransform = Utilities::GetGeometryTransformation(inNode);

	// A deformer is a FBX thing, which contains some clusters
	// A cluster contains a link, which is basically a joint
	// Normally, there is only one deformer in a mesh
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		// There are many types of deformers in Maya,
		// We are using only skins, so we see if this is a skin
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		_boneOffset.resize(_bones->size());

		unsigned int numOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			/*unsigned int currJointIndex = FindJointIndexUsingName(currJointName);*/
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
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() *transformMatrix;// *geometryTransform;

			// Update the information in mSkeleton 
			_boneOffset[currJointIndex] = FbxDxUtil::ToDXMatrix(globalBindposeInverseMatrix);
			//_bones->data()[currJointIndex]->SetLocalMatrix(FbxDxUtil::ToDXMatrix(globalBindposeInverseMatrix));
			/*mSkeleton.mJoints[currJointIndex].mGlobalBindposeInverse = globalBindposeInverseMatrix;
			mSkeleton.mJoints[currJointIndex].mNode = currCluster->GetLink();*/

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				BlendingIndexWeightPair currBlendingIndexWeightPair;
				currBlendingIndexWeightPair.mBlendingIndex = currJointIndex;
				currBlendingIndexWeightPair.mBlendingWeight = (float)currCluster->GetControlPointWeights()[i];
				mControlPoints[currCluster->GetControlPointIndices()[i]]->mBlendingInfo.push_back(currBlendingIndexWeightPair);
			}
		}
	}

	// Some of the control points only have less than 4 joints
	// affecting them.
	// For a normal renderer, there are usually 4 joints
	// I am adding more dummy joints if there isn't enough
	BlendingIndexWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.mBlendingIndex = 0;
	currBlendingIndexWeightPair.mBlendingWeight = 0;
	for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		for (unsigned int i = itr->second->mBlendingInfo.size(); i <= 4; ++i)
		{
			itr->second->mBlendingInfo.push_back(currBlendingIndexWeightPair);
		}
	}
}
unsigned int SkinnedMesh::FindJointIndexUsingName(const std::string& inJointName)
{
	/*for (unsigned int i = 0; i < mSkeleton.mJoints.size(); ++i)
	{
		if (mSkeleton.mJoints[i].mName.compare(inJointName) == 0)
		{
			return i;
		}
	}

	throw std::exception("Skeleton information in FBX file is corrupted.");*/
	return 0;
}



//void SkinnedMesh::ReadBinormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, D3DXVECTOR3& outBinormal)
//{
//	if (inMesh->GetElementBinormalCount() < 1)
//	{
//		throw std::exception("Invalid Binormal Number");
//	}
//
//	FbxGeometryElementBinormal* vertexBinormal = inMesh->GetElementBinormal(0);
//	switch (vertexBinormal->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (vertexBinormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
//			outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
//			outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexBinormal->GetIndexArray().GetAt(inCtrlPointIndex);
//			outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
//			outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
//			outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//
//		default:
//			throw std::exception("Invalid Reference");
//		}
//		break;
//
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (vertexBinormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
//			outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
//			outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexBinormal->GetIndexArray().GetAt(inVertexCounter);
//			outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
//			outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
//			outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//
//		default:
//			throw std::exception("Invalid Reference");
//		}
//		break;
//	}
//}

void SkinnedMesh::Optimize()
{
	// First get a list of unique vertices
	std::vector<PNTIWVertex> uniqueVertices;
	for (unsigned int i = 0; i < mTriangles.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			// If current vertex has not been added to
			// our unique vertex list, then we add it
			if (FindVertex(mVertices[i * 3 + j], uniqueVertices) == -1)
			{
				uniqueVertices.push_back(mVertices[i * 3 + j]);
			}
		}
	}

	// Now we update the index buffer
	for (unsigned int i = 0; i < mTriangles.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			mTriangles[i].mIndices[j] = FindVertex(mVertices[i * 3 + j], uniqueVertices);
		}
	}

	mVertices.clear();
	mVertices = uniqueVertices;
	uniqueVertices.clear();

	// Now we sort the triangles by materials to reduce 
	// shader's workload
	std::sort(mTriangles.begin(), mTriangles.end());
}
int SkinnedMesh::FindVertex(const PNTIWVertex& inTargetVertex, const std::vector<PNTIWVertex>& uniqueVertices)
{
	for (unsigned int i = 0; i < uniqueVertices.size(); ++i)
	{
		if (inTargetVertex == uniqueVertices[i])
		{
			return i;
		}
	}

	return -1;
}


















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SkinnedMesh::LoadSkinInfo(FbxNode& fbxNode, std::vector<SKINNEDMESHVERTEX>& bufferVertex)
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
		D3DXMATRIX identity(0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
		D3DXMatrixIdentity(&identity);

		for(int i = 0; i < _boneOffset.size(); ++i)
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

			FbxVector4 lT, lR, lS;
			FbxAMatrix currentTransformAtT = currCluster->GetLink()->EvaluateGlobalTransform(0);
			lT = currentTransformAtT.GetT();
			lR = currentTransformAtT.GetR();
			lS = currentTransformAtT.GetS();

			currentTransformAtT.SetTRS(lT, lR, lS);

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * t;
			FbxAMatrix finalTransform = currentTransformAtT * globalBindposeInverseMatrix;
			lT = finalTransform.GetT();
			lR = finalTransform.GetR();
			lS = finalTransform.GetS();
			lT[0] *= -1;
			lR[1] *= -1;
			lR[2] *= -1;
			finalTransform.SetTRS(lT, lR, lS);

			
			// Update the information in mSkeleton 
			_boneOffset[currJointIndex] = FbxDxUtil::ToDXMatrix(globalBindposeInverseMatrix);
			//_boneOffset[currJointIndex] = FbxDxUtil::ToDXMatrix(finalTransform);

			double *pWeights = currCluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
			int *pIndices = currCluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들																				
			int numBoneVertexIndices = currCluster->GetControlPointIndicesCount(); // 해당 본에 영향을 받는 모든 정점을 하나씩 가져옴	
			
			for (int k = 0; k < numBoneVertexIndices; k++)
			{
				float tempWeight = (float)pWeights[k];	// 영향을 받는 정점의 가중치 정도
				int tempIndex = pIndices[k];			// 영향을 받는 정점의 인덱스
			
				// 가중치가 0이면 다음걸로 넘김
				if (tempWeight == 0) continue;
							
				// 가중치 중 x가 0이면 첫번째 인덱스
				if (bufferVertex[tempIndex].Weight.x == 0)
				{
					bufferVertex[tempIndex].Weight.x = tempWeight;
					bufferVertex[tempIndex].Index[0] = currJointIndex;
				}
				// 가중치 중 x가 0이 아니고 y가 0이면 두번째 인덱스
				else if (bufferVertex[tempIndex].Weight.x != 0 &&
						bufferVertex[tempIndex].Weight.y == 0)
				{
					bufferVertex[tempIndex].Weight.y = tempWeight;
					bufferVertex[tempIndex].Index[1] = currJointIndex;
				}
				// 가중치 중 x가 0이 아니고 y가 0이 아니고 z가 0이면 세번째 인덱스
				else if (bufferVertex[tempIndex].Weight.x != 0 &&
						bufferVertex[tempIndex].Weight.y != 0 &&
						bufferVertex[tempIndex].Weight.z == 0)
				{					
					bufferVertex[tempIndex].Weight.z = tempWeight;
					bufferVertex[tempIndex].Index[2] = currJointIndex;
				}
				// 모두 0이 아니면 4번째 인덱스, 가중치는 1에서 xyz 빼면 나머지 구할 수 있음
				else if (bufferVertex[tempIndex].Weight.x!= 0 &&
						bufferVertex[tempIndex].Weight.y != 0 &&
						bufferVertex[tempIndex].Weight.z != 0)
				{
					bufferVertex[tempIndex].Index[3] = currJointIndex;
				}
			} // k

		}
	}
}

void SkinnedMesh::LoadPosition(FbxMesh& mesh, int ctrlPointIndex, D3DXVECTOR3& position)
{
	FbxVector4* pCtrlPoint = mesh.GetControlPoints();

	position.x = (float)pCtrlPoint[ctrlPointIndex].mData[0];
	position.y = (float)pCtrlPoint[ctrlPointIndex].mData[1];
	position.z = (float)pCtrlPoint[ctrlPointIndex].mData[2];
}

void SkinnedMesh::LoadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR4& color)
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
void SkinnedMesh::LoadUV(FbxMesh& mesh, int ctrlPointIndex, int uvIndex, D3DXVECTOR2& uv)
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
void SkinnedMesh::LoadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& normal)
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
void SkinnedMesh::LoadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& tangent)
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
