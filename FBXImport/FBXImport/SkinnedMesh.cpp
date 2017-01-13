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
		//*_boneOffset[idx];//
		m = matLocal *_bones->data()[idx]->GetWorldMatrix();
		m *= matWorld;
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

			bufferVertex.push_back(vertex);
			bufferIndex.push_back(vertexCount);

			vertexCount++;
		}
	}

	if (pData)
	{
		_bones = static_cast<std::vector<Frame *>*>(pData);

		LoadSkinInfo(fbxNode, bufferVertex);

		std::set<int> indexBone;
		
		for (int i = 0; i < bufferVertex.size(); i++) {
			for (int j = 0; j < 4; j++)	{
				indexBone.insert(bufferVertex[i].Index[j]);
			}					
		}
		
		for (auto it = indexBone.begin(); it != indexBone.end(); ++it) {
			_indexBones.push_back(*it);
		}
	}
	
	

	// 정점 버퍼 정보 채우기
	_vertexCount = bufferVertex.size();
	int bufferVertexSize = _vertexCount * sizeof(SKINNEDMESHVERTEX);

	if (!_device)
		DebugBox(0, 0);

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
	for (DWORD i = 0; i < dwDeformerCnt; ++i)
	{
		// There are many types of deformers in Maya,
		// We are using only skins, so we see if this is a skin
		FbxSkin* currSkin = reinterpret_cast< FbxSkin* >(currMesh->GetDeformer(i, FbxDeformer::eSkin));
		if (!currSkin)	continue;

		_boneOffset.resize(_bones->size());
		

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

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;
			FbxVector4 vS = globalBindposeInverseMatrix.GetS();
			globalBindposeInverseMatrix.SetS(FbxVector4(vS.mData[0], -vS.mData[1], vS.mData[2], vS.mData[3]));

			
			// Update the information in mSkeleton 
			_boneOffset[currJointIndex] = FbxDxUtil::ToDXMatrix(globalBindposeInverseMatrix);

			// Associate each joint with the control points it affects
			/*DWORD dwIndexCnt = (DWORD)currCluster->GetControlPointIndicesCount();
			
			for (DWORD k = 0; k < dwIndexCnt; ++k)
			{			
				bufferVertex[currCluster->GetControlPointIndices()[k]].Weight[k] = currCluster->GetControlPointWeights()[k];
				bufferVertex[currCluster->GetControlPointIndices()[k]].Index[k] = currJointIndex;
			}*/
			double *pWeights = currCluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
			int *pIndices = currCluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들																				
			int numBoneVertexIndices = currCluster->GetControlPointIndicesCount(); // 해당 본에 영향을 받는 모든 정점을 하나씩 가져옴	
			
			for (int k = 0; k < numBoneVertexIndices; k++)
			{
				float tempWeight = (float)pWeights[k];	// 영향을 받는 정점의 가중치 정도
			
				// 가중치가 0이면 다음걸로 넘김
				if (!tempWeight)
					continue;
			
				int tempIndex = pIndices[k];			// 영향을 받는 정점의 인덱스
							
				// 가중치 중 x가 0이면 첫번째 인덱스
				if (bufferVertex[tempIndex].Weight[0] == 0)
				{
					bufferVertex[tempIndex].Weight[0] = tempWeight;
					bufferVertex[tempIndex].Index[0] = currJointIndex;
				}
				// 가중치 중 x가 0이 아니고 y가 0이면 두번째 인덱스
				else if (bufferVertex[tempIndex].Weight[0] != 0 &&
						bufferVertex[tempIndex].Weight[1] == 0)
				{
					bufferVertex[tempIndex].Weight[1] = tempWeight;
					bufferVertex[tempIndex].Index[1] = currJointIndex;
				}
				// 가중치 중 x가 0이 아니고 y가 0이 아니고 z가 0이면 세번째 인덱스
				else if (bufferVertex[tempIndex].Weight[0] != 0 &&
						bufferVertex[tempIndex].Weight[1] != 0 &&
						bufferVertex[tempIndex].Weight[2] == 0)
				{
					bufferVertex[tempIndex].Weight[2] = tempWeight;
					bufferVertex[tempIndex].Index[2] = currJointIndex;
				}
				// 모두 0이 아니면 4번째 인덱스, 가중치는 1에서 xyz 빼면 나머지 구할 수 있음
				else if (bufferVertex[tempIndex].Weight[0] != 0 &&
						bufferVertex[tempIndex].Weight[1] != 0 &&
						bufferVertex[tempIndex].Weight[2] != 0)
				{
					bufferVertex[tempIndex].Index[3] = currJointIndex;
				}
			} // k

		}
	}

	// Some of the control points only have less than 4 joints
	// affecting them.
	// For a normal renderer, there are usually 4 joints
	// I am adding more dummy joints if there isn't enough
	/*BLENDINFO currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.Index = 0;
	currBlendingIndexWeightPair.Weight = 0;

	for (size_t i = 0; i < _ctrlPoint.size(); ++i)
	{
		for (size_t j = _ctrlPoint[i].BlendInfo.size(); j < 4; ++j)
		{
			_ctrlPoint[i].BlendInfo.push_back(currBlendingIndexWeightPair);
		}
	}*/
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

//void SkinnedMesh::LoadPosition(FbxMesh& mesh, int ctrlPointIndex, D3DXVECTOR3& position)
//{
//	FbxVector4* pCtrlPoint = mesh.GetControlPoints();
//
//	position.x = (float)pCtrlPoint[ctrlPointIndex].mData[0];
//	position.y = (float)pCtrlPoint[ctrlPointIndex].mData[1];
//	position.z = (float)pCtrlPoint[ctrlPointIndex].mData[2];
//}
//void SkinnedMesh::LoadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR4& color)
//{
//	if (mesh.GetElementVertexColorCount() < 1) return;
//
//	FbxGeometryElementVertexColor* pVertexColor = mesh.GetElementVertexColor(0);
//
//	FbxLayerElement::EMappingMode mappingMode = pVertexColor->GetMappingMode();
//	FbxLayerElement::EReferenceMode referenceMode = pVertexColor->GetReferenceMode();
//
//	switch (mappingMode)
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			color.x = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
//			color.y = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
//			color.z = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
//			color.w = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
//			color.x = (float)pVertexColor->GetDirectArray().GetAt(i).mRed;
//			color.y = (float)pVertexColor->GetDirectArray().GetAt(i).mGreen;
//			color.z = (float)pVertexColor->GetDirectArray().GetAt(i).mBlue;
//			color.w = (float)pVertexColor->GetDirectArray().GetAt(i).mAlpha;
//		}
//		break;
//		}
//
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			color.x = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mRed;
//			color.y = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mGreen;
//			color.z = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mBlue;
//			color.w = (float)pVertexColor->GetDirectArray().GetAt(vertexCount).mAlpha;
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pVertexColor->GetIndexArray().GetAt(vertexCount);
//			color.x = (float)pVertexColor->GetDirectArray().GetAt(i).mRed;
//			color.y = (float)pVertexColor->GetDirectArray().GetAt(i).mGreen;
//			color.z = (float)pVertexColor->GetDirectArray().GetAt(i).mBlue;
//			color.w = (float)pVertexColor->GetDirectArray().GetAt(i).mAlpha;
//		}
//		break;
//		}
//		break;
//	}
//}
//void SkinnedMesh::LoadUV(FbxMesh& mesh, int ctrlPointIndex, int uvIndex, D3DXVECTOR2& uv)
//{
//	if (mesh.GetElementUVCount() < 1) return;
//
//	FbxGeometryElementUV* pUV = mesh.GetElementUV(0);
//
//	FbxLayerElement::EMappingMode mappingMode = pUV->GetMappingMode();
//	FbxLayerElement::EReferenceMode referenceMode = pUV->GetReferenceMode();
//
//	switch (mappingMode)
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			uv.x = (float)pUV->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
//			uv.y = (float)pUV->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pUV->GetIndexArray().GetAt(ctrlPointIndex);
//			uv.x = (float)pUV->GetDirectArray().GetAt(i).mData[0];
//			uv.y = (float)pUV->GetDirectArray().GetAt(i).mData[1];
//		}
//		break;
//		}
//
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//			break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			uv.x = (float)pUV->GetDirectArray().GetAt(uvIndex).mData[0];
//			uv.y = (float)pUV->GetDirectArray().GetAt(uvIndex).mData[1];
//		}
//		break;
//		}
//		break;
//	}
//}
//void SkinnedMesh::LoadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& normal)
//{
//	if (mesh.GetElementNormalCount() < 1) return;
//
//	FbxGeometryElementNormal* pNormal = mesh.GetElementNormal(0);
//
//	FbxLayerElement::EMappingMode mappingMode = pNormal->GetMappingMode();
//	FbxLayerElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();
//
//	switch (mappingMode)
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			normal.x = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
//			normal.y = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
//			normal.z = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pNormal->GetIndexArray().GetAt(ctrlPointIndex);
//			normal.x = (float)pNormal->GetDirectArray().GetAt(i).mData[0];
//			normal.y = (float)pNormal->GetDirectArray().GetAt(i).mData[1];
//			normal.z = (float)pNormal->GetDirectArray().GetAt(i).mData[2];
//		}
//		break;
//		}
//
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			normal.x = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[0];
//			normal.y = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[1];
//			normal.z = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[2];
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pNormal->GetIndexArray().GetAt(vertexCount);
//			normal.x = (float)pNormal->GetDirectArray().GetAt(i).mData[0];
//			normal.y = (float)pNormal->GetDirectArray().GetAt(i).mData[1];
//			normal.z = (float)pNormal->GetDirectArray().GetAt(i).mData[2];
//		}
//		break;
//		}
//		break;
//	}
//}
//void SkinnedMesh::LoadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& tangent)
//{
//	if (mesh.GetElementTangentCount() < 1) return;
//
//	FbxGeometryElementTangent* pTangent = mesh.GetElementTangent(0);
//
//	FbxLayerElement::EMappingMode mappingMode = pTangent->GetMappingMode();
//	FbxLayerElement::EReferenceMode referenceMode = pTangent->GetReferenceMode();
//
//	switch (mappingMode)
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			tangent.x = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
//			tangent.y = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
//			tangent.z = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pTangent->GetIndexArray().GetAt(ctrlPointIndex);
//			tangent.x = (float)pTangent->GetDirectArray().GetAt(i).mData[0];
//			tangent.y = (float)pTangent->GetDirectArray().GetAt(i).mData[1];
//			tangent.z = (float)pTangent->GetDirectArray().GetAt(i).mData[2];
//		}
//		break;
//		}
//
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (referenceMode)
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			tangent.x = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[0];
//			tangent.y = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[1];
//			tangent.z = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[2];
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int i = pTangent->GetIndexArray().GetAt(vertexCount);
//			tangent.x = (float)pTangent->GetDirectArray().GetAt(i).mData[0];
//			tangent.y = (float)pTangent->GetDirectArray().GetAt(i).mData[1];
//			tangent.z = (float)pTangent->GetDirectArray().GetAt(i).mData[2];
//		}
//		break;
//		}
//		break;
//	}
//}

//void SkinnedMesh::LoadSkinInfo(FbxMesh& mesh, std::vector<SKINNEDMESHVERTEX>& bufferVertex)
//{
//	int deformerCount = mesh.GetDeformerCount(FbxDeformer::eSkin);
//
//	if (mesh.GetDeformerCount() < 1) return;
//	
//	for (int i = 0; i < deformerCount; ++i)
//	{
//		FbxVector4* ctrlPoint = mesh.GetControlPoints();
//
//		int vertexCount = mesh.GetControlPointsCount();
//		std::vector<SKINNEDMESHVERTEX> indexedVertice;
//
//		for (int j = 0; j < vertexCount; ++j)
//		{
//			SKINNEDMESHVERTEX vertex;
//
//			vertex.Position = FbxDxUtil::ToDXVector3(ctrlPoint[j]);			
//			indexedVertice.push_back(vertex);
//		}
//
//		_boneOffset.resize(_bones->size());
//
//		FbxSkin* pSkin = FbxCast<FbxSkin>(mesh.GetDeformer(i, FbxDeformer::eSkin));
//
//		if (!pSkin) continue;
//
//		int clusterCount = pSkin->GetClusterCount();
//		
//		for (int j = 0; j < clusterCount; ++j)
//		{
//			FbxCluster* pCluster = pSkin->GetCluster(j);
//
//			FbxAMatrix matrix;
//			FbxAMatrix linkMatrix;
//
//			pCluster->GetTransformMatrix(matrix);
//			pCluster->GetTransformLinkMatrix(linkMatrix);
//			FbxAMatrix bindposeMatrix = linkMatrix.Inverse() * matrix;	// 오프셋 매트릭스
//
//			int boneIndex = 0;
//			std::string name = pCluster->GetLink()->GetName();
//			
//			for (int i = 0; i < _bones->size(); ++i)
//			{
//				if (name.compare(_bones->data()[i]->GetName()) == 0)
//				{
//					boneIndex = i;	// 인덱스
//					break;
//				}
//			}
//			
//			_boneOffset[boneIndex] = FbxDxUtil::ToDXMatrix(bindposeMatrix);
//
//			double *pWeights = pCluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
//			int *pIndices = pCluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들																				
//			int numBoneVertexIndices = pCluster->GetControlPointIndicesCount(); // 해당 본에 영향을 받는 모든 정점을 하나씩 가져옴	
//
//			for (int k = 0; k < numBoneVertexIndices; k++)
//			{
//				float tempWeight = (float)pWeights[k];	// 영향을 받는 정점의 가중치 정도
//
//				// 가중치가 0이면 다음걸로 넘김
//				if (!tempWeight)
//					continue;
//
//				int tempIndex = pIndices[k];			// 영향을 받는 정점의 인덱스
//				
//				// 가중치 중 x가 0이면 첫번째 인덱스
//				if (indexedVertice[tempIndex].Weight[0] == 0)
//				{
//					indexedVertice[tempIndex].Weight[0] = tempWeight;
//					indexedVertice[tempIndex].Index[0] = boneIndex;
//				}
//				// 가중치 중 x가 0이 아니고 y가 0이면 두번째 인덱스
//				else if (indexedVertice[tempIndex].Weight[0] != 0 &&
//						 indexedVertice[tempIndex].Weight[1] == 0)
//				{
//					indexedVertice[tempIndex].Weight[1] = tempWeight;
//					indexedVertice[tempIndex].Index[1] = boneIndex;
//				}
//				// 가중치 중 x가 0이 아니고 y가 0이 아니고 z가 0이면 세번째 인덱스
//				else if (indexedVertice[tempIndex].Weight[0] != 0 &&
//						 indexedVertice[tempIndex].Weight[1] != 0 &&
//						 indexedVertice[tempIndex].Weight[2] == 0)
//				{
//					indexedVertice[tempIndex].Weight[2] = tempWeight;
//					indexedVertice[tempIndex].Index[2] = boneIndex;
//				}
//				// 모두 0이 아니면 4번째 인덱스, 가중치는 1에서 xyz 빼면 나머지 구할 수 있음
//				else if (indexedVertice[tempIndex].Weight[0] != 0 &&
//						 indexedVertice[tempIndex].Weight[1] != 0 &&
//						 indexedVertice[tempIndex].Weight[2] != 0)
//				{
//					indexedVertice[tempIndex].Index[3] = boneIndex;
//				}
//			} // k
//		} // j
//
//		for (int j = 0; j < bufferVertex.size(); j++)
//		{			
//			SKINNEDMESHVERTEX tempVertex = bufferVertex.data()[j];
//			for (int k = 0; k < indexedVertice.size(); k++)
//			{
//				SKINNEDMESHVERTEX tempVertex2 = indexedVertice[k];
//
//				if (tempVertex.Position.x == tempVertex2.Position.x &&
//					tempVertex.Position.y == tempVertex2.Position.y &&
//					tempVertex.Position.z == tempVertex2.Position.z)
//				{
//					bufferVertex.data()[j].Weight[0] = indexedVertice[k].Weight[0];
//					bufferVertex.data()[j].Weight[1] = indexedVertice[k].Weight[1];
//					bufferVertex.data()[i].Weight[2] = indexedVertice[k].Weight[2];
//
//					bufferVertex.data()[j].Index[0] = indexedVertice[k].Index[0];
//					bufferVertex.data()[j].Index[1] = indexedVertice[k].Index[1];
//					bufferVertex.data()[j].Index[2] = indexedVertice[k].Index[2];
//					bufferVertex.data()[j].Index[3] = indexedVertice[k].Index[3];
//					break;
//				}
//			} // k
//			
//		} // j
//	} // i
//
//}

//SkinnedMesh::SkinnedMesh(IDirect3DDevice9& device)
//	: Mesh(device)
//{
//	_isSkinned = true;
//	_vb = new VertexBuffer(device);
//	_ib = new IndexBuffer(device);
//}
//
//
//SkinnedMesh::~SkinnedMesh()
//{
//	_boneHierarchy = nullptr;
//	SAFE_DELETE(_vb);
//	SAFE_DELETE(_ib);
//}
//
//void SkinnedMesh::Load(FbxNode& fbxNode, void* pData)
//{
//	LoadVerticeAndIndice(fbxNode, pData);
//}
//
//void SkinnedMesh::Save(FbxNode& fbxNode)
//{
//
//}
//
//void SkinnedMesh::Draw()
//{
//	assert(_vb->GetVertexBuffer() && "vertex buffer doesn't created!!");
//	assert(_ib->GetData() && "index buffer doesn't created!!");
//
//	//_device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
//	//_device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);
//
//	_device->SetStreamSource(0, _vb->GetVertexBuffer(), 0, sizeof(SKINNEDMESHVERTEX));
//	_device->SetVertexDeclaration(_vb->GetVertexDeclaration());
//	_device->SetIndices(_ib->GetData());
//	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertexCount, 0, _primitiveCount);
//
//	//_device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
//	//_device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
//}
//
//void SkinnedMesh::ApplyMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matLocal)
//{
//	int size = _indexBones.size();
//	int idx = 0;
//
//	D3DXMATRIX m;
//
//	for (int i = 0; i < size; i++)
//	{
//		idx = _indexBones[i];
//		D3DXMATRIX inverse = _boneHierarchy->data()[idx]->GetWorldMatrixInverse();
//		D3DXMATRIX temp = inverse *inverse; // * 
//		m = *matLocal *_boneOffset[idx];// *inverse;
//		m *= *matWorld;
//		_device->SetTransform(D3DTS_WORLDMATRIX(idx), &m);
//	}
//}
//
//void SkinnedMesh::LoadVerticeAndIndice(FbxNode& fbxNode, void* pData)
//{
//	FbxMesh* pMesh = fbxNode.GetMesh();
//
//	std::vector<SKINNEDMESHVERTEX>		bufferVertice;
//	std::vector<unsigned long>			bufferIndice;
//
//	int index = 0;
//	int polygonCount = pMesh->GetPolygonCount();
//
//	// 버텍스 포지션, 노멀, 텍스쳐 좌표 파싱.
//	for (int i = 0; i < polygonCount; i++)
//	{
//		int numVertices = pMesh->GetPolygonSize(i);
//
//		for (int j = 0; j < numVertices; j++)			// numVertices가 3인지 4일지 알 수가 없다 -> size를 알 수 없다. vector말고 array를 사용하지 못한다.
//		{
//			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
//
//			SKINNEDMESHVERTEX vertex;
//			vertex.Position.x = (float)pMesh->GetControlPointAt(controlPointIndex).mData[0];
//			vertex.Position.y = (float)pMesh->GetControlPointAt(controlPointIndex).mData[1];
//			vertex.Position.z = (float)pMesh->GetControlPointAt(controlPointIndex).mData[2];
//
//			FbxVector4 normal;
//			LoadNormal(*pMesh, controlPointIndex, index, normal);
//			vertex.Normal.x = (float)normal.mData[0];
//			vertex.Normal.y = (float)normal.mData[1];
//			vertex.Normal.z = (float)normal.mData[2];
//
//			FbxVector2 uv;
//			LoadTexCoord(*pMesh, controlPointIndex, index, uv);
//			vertex.TexCoord.x = (float)uv.mData[0];
//			vertex.TexCoord.y = (float)uv.mData[1];
//
//			bufferVertice.push_back(vertex);
//			bufferIndice.push_back(index++);
//		}
//	}
//
//	if (pData)
//	{
//		_boneHierarchy = static_cast<std::vector<Frame *>*>(pData);
//		LoadSkinInfo(*pMesh, bufferVertice, *_boneHierarchy);	// 가중치 및 본 인덱스 파싱.
//
//		std::set<int> indexBone;
//
//		for (int i = 0; i < bufferVertice.size(); i++)
//		{
//			for (int j = 0; j < 4; j++)
//			{
//				indexBone.insert(bufferVertice[i].Index[j]);
//			}
//			
//		}
//
//		for (auto it = indexBone.begin(); it != indexBone.end(); ++it)
//		{
//			_indexBones.push_back(*it);
//		}
//	}
//
//	const D3DVERTEXELEMENT9 dec[6] =
//	{
//		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,0 },
//		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0 },
//		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,0 },
//		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,0 },
//		{ 0, 44, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0 },
//		D3DDECL_END()
//	};
//	
//	_vertexCount = bufferVertice.size();
//	int bufferVerticeSize = _vertexCount * sizeof(SKINNEDMESHVERTEX);
//	if (bufferVerticeSize > 0)					// CreateVertexBuffer일 때 사이즈가 0이면 에러가 뜸.
//	{
//		_vb->Create(bufferVerticeSize, 0, D3DPOOL_MANAGED);
//		_vb->CreateVertexDeclaration(dec);
//
//		void* pData;
//
//		if (_vb->Lock(0, bufferVerticeSize, &pData, 0))
//		{
//			memcpy(pData, &bufferVertice[0], bufferVerticeSize);
//
//			_vb->Unlock();
//		}
//	}
//
//	// 16bit 인덱스를 쓸건지 32bit 인덱스를 쓸건지 Device caps를 통해서 알아내기
//	_primitiveCount = bufferIndice.size() / 3;
//	int bufferIndiceSize = bufferIndice.size() * sizeof(unsigned long);
//	if (bufferIndiceSize > 0)
//	{
//		_ib->Create(bufferIndiceSize, 0, D3DPOOL_MANAGED);
//		void * pData;
//		if (_ib->Lock(0, bufferIndiceSize, &pData, 0))
//		{
//			memcpy(pData, &bufferIndice[0], bufferIndiceSize);
//
//			_ib->Unlock();
//		}
//	}
//}
//
//
//void SkinnedMesh::LoadTexCoord(FbxMesh& fbxMesh, int index, int vertexId, FbxVector2& texCoord)
//{
//	FbxStringList nameList;
//	fbxMesh.GetUVSetNames(nameList);
//
//	const char* name = nameList.GetStringAt(0);
//	const FbxGeometryElementUV* uvElement = fbxMesh.GetElementUV(name);
//
//	if (!uvElement) return;
//
//	FbxLayerElement::EMappingMode mapMode = uvElement->GetMappingMode();
//	FbxLayerElement::EReferenceMode refMode = uvElement->GetReferenceMode();
//
//	switch (mapMode)
//	{
//	case FbxGeometryElement::eByControlPoint:
//		if (refMode == FbxGeometryElement::eDirect)
//		{
//			texCoord = uvElement->GetDirectArray().GetAt(index);
//		}
//		else if (refMode == FbxGeometryElement::eIndexToDirect)
//		{
//			int i = uvElement->GetIndexArray().GetAt(index);
//			texCoord = uvElement->GetDirectArray().GetAt(i);
//		}
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		if (refMode == FbxGeometryElement::eDirect)
//		{
//			texCoord = uvElement->GetDirectArray().GetAt(vertexId);
//		}
//		else if (refMode == FbxGeometryElement::eIndexToDirect)
//		{
//			int i = uvElement->GetIndexArray().GetAt(vertexId);
//			texCoord = uvElement->GetDirectArray().GetAt(i);
//		}
//		break;
//	}
//}
//
//void SkinnedMesh::LoadNormal(FbxMesh& fbxMesh, int index, int vertexId, FbxVector4& normal)
//{
//	FbxGeometryElementNormal* normalElement = fbxMesh.GetElementNormal(0);
//
//	if (!normalElement) return;
//
//	FbxLayerElement::EMappingMode mapMode = normalElement->GetMappingMode();
//	FbxLayerElement::EReferenceMode refMode = normalElement->GetReferenceMode();
//	
//	switch (mapMode)
//	{
//	case FbxGeometryElement::eByControlPoint:
//		if (refMode == FbxGeometryElement::eDirect)
//		{
//			normal = normalElement->GetDirectArray().GetAt(index);
//		}
//		else if (refMode == FbxGeometryElement::eIndexToDirect)
//		{
//			int i = normalElement->GetIndexArray().GetAt(index);
//			normal = normalElement->GetDirectArray().GetAt(i);
//		}
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		if (refMode == FbxGeometryElement::eDirect)
//		{
//			normal = normalElement->GetDirectArray().GetAt(vertexId);
//		}
//		else if (refMode == FbxGeometryElement::eIndexToDirect)
//		{
//			int i = normalElement->GetIndexArray().GetAt(vertexId);
//			normal = normalElement->GetDirectArray().GetAt(i);
//		}
//		break;
//	}
//}
//
//void SkinnedMesh::LoadSkinInfo(FbxMesh& fbxMesh, std::vector<SKINNEDMESHVERTEX>& bufferVertice, std::vector<Frame *>& boneHierarchy)
//{
//	int i = fbxMesh.GetDeformerCount(FbxDeformer::eSkin);
//
//	for (int i = 0; i < i; i++)
//	{
//		FbxVector4* pIndexedVertex = fbxMesh.GetControlPoints();
//
//		int vertexCount = fbxMesh.GetControlPointsCount();
//		std::vector<SKINNEDMESHVERTEX> indexedVertice;
//
//		for (int j = 0; j < vertexCount; j++)
//		{
//			SKINNEDMESHVERTEX vertex;
//			vertex.Position.x = (float)pIndexedVertex[j].mData[0];
//			vertex.Position.y = (float)pIndexedVertex[j].mData[1];
//			vertex.Position.z = (float)pIndexedVertex[j].mData[2];
//
//			indexedVertice.push_back(vertex);
//		}
//		
//		FbxSkin* skin = FbxCast<FbxSkin>(fbxMesh.GetDeformer(i, FbxDeformer::eSkin));
//
//		if (!skin) continue;
//
//		_boneOffset.resize(boneHierarchy.size());
//
//		int boneCount = skin->GetClusterCount();
//
//		for (int j = 0; j < boneCount; j++)
//		{
//			FbxCluster* cluster = skin->GetCluster(j);
//			FbxNode* bone = cluster->GetLink();
//			int boneIndex = 0;
//
//			for (UINT k = 0; k < boneHierarchy.size(); k++)
//			{				
//				if (strcmp(bone->GetName(), boneHierarchy[k]->GetName()) == 0)
//				{
//					boneIndex = k;
//					break;
//				}
//			}
//
//			FbxAMatrix LinkBoneMatrix;
//			FbxAMatrix TransboneMatrix;
//			FbxAMatrix ResultMatrix;
//
//			cluster->GetTransformLinkMatrix( LinkBoneMatrix );
//			cluster->GetTransformMatrix( TransboneMatrix );
//			ResultMatrix = LinkBoneMatrix.Inverse()  *TransboneMatrix;			
//
//			_boneOffset[boneIndex] = FbxDxUtil::ToDXMatrix(ResultMatrix);
//
//			double *boneVertexWeights = cluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
//			int *boneVertexIndices = cluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들
//																				// 해당 본에 영향을 받는 모든 정점을 하나씩 가져옴	
//			int numBoneVertexIndices = cluster->GetControlPointIndicesCount();
//						
//			for (int k = 0; k < numBoneVertexIndices; k++)
//			{
//				float tempBoneWeight = (float)boneVertexWeights[k];	// 영향을 받는 정점의 가중치 정도
//																					// 가중치가 0이면 다음걸로 넘김
//				if (!tempBoneWeight) continue;
//
//				int tempBoneVertexIndex = boneVertexIndices[k];			// 영향을 받는 정점의 인덱스
//
//				// 가중치 중 x가 0이면 첫번째 인덱스
//				if (indexedVertice[k].Weight[0] == 0)
//				{
//					indexedVertice[k].Weight[0] = tempBoneWeight;
//					indexedVertice[k].Index[0] = boneIndex;
//				}
//				// 가중치 중 x가 0이 아니고 y가 0이면 두번째 인덱스
//				else if (indexedVertice[k].Weight[0] != 0 &&
//						 indexedVertice[k].Weight[1] == 0)
//				{
//					indexedVertice[k].Weight[1] = tempBoneWeight;
//					indexedVertice[k].Index[1] = boneIndex;
//				}
//				// 가중치 중 x가 0이 아니고 y가 0이 아니고 z가 0이면 세번째 인덱스
//				else if (indexedVertice[k].Weight[0] != 0 &&
//						 indexedVertice[k].Weight[1] != 0 &&
//						 indexedVertice[k].Weight[2] == 0)
//				{
//					indexedVertice[k].Weight[2] = tempBoneWeight;
//					indexedVertice[k].Index[2] = boneIndex;
//				}
//				// 모두 0이 아니면 4번째 인덱스, 가중치는 1에서 xyz 빼면 나머지 구할 수 있음
//				else if (indexedVertice[k].Weight[0] != 0 &&
//						 indexedVertice[k].Weight[1] != 0 &&
//						 indexedVertice[k].Weight[2] != 0)
//				{
//					indexedVertice[k].Index[3] = boneIndex;
//				}
//			} 
//		}
//
//		for (int j = 0; j < bufferVertice.size(); j++)
//		{
//			if (bufferVertice[j].Index[0] == 0 && bufferVertice[j].Weight[0] == 0)
//			{
//				SKINNEDMESHVERTEX tempVertex = bufferVertice[j];
//				for (int k = 0; k < indexedVertice.size(); k++)
//				{
//					SKINNEDMESHVERTEX tempVertex2 = indexedVertice[k];
//
//					if (tempVertex.Position == tempVertex2.Position)
//					{
//						bufferVertice[j].Weight[0] = indexedVertice[k].Weight[0];
//						bufferVertice[j].Weight[1] = indexedVertice[k].Weight[1];
//						bufferVertice[j].Weight[2] = indexedVertice[k].Weight[2];
//
//						bufferVertice[j].Index[0] = indexedVertice[k].Index[0];
//						bufferVertice[j].Index[1] = indexedVertice[k].Index[1];
//						bufferVertice[j].Index[2] = indexedVertice[k].Index[2];
//						bufferVertice[j].Index[3] = indexedVertice[k].Index[3];
//						break;
//					}
//				}
//			}
//		}
//	}
//}