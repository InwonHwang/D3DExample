#include "SkinnedMesh.h"
#include "FbxDXUtil.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Frame.h"

SkinnedMesh::SkinnedMesh(IDirect3DDevice9& device)
	: Mesh(device)
{
	_isSkinned = true;
	_vb = new VertexBuffer(device);
	_ib = new IndexBuffer(device);
}


SkinnedMesh::~SkinnedMesh()
{
	_boneHierarchy = nullptr;
	SAFE_DELETE(_vb);
	SAFE_DELETE(_ib);
}

void SkinnedMesh::Load(FbxNode& fbxNode, void* pData)
{
	LoadVerticeAndIndice(fbxNode, pData);
}

void SkinnedMesh::Save(FbxNode& fbxNode)
{

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

void SkinnedMesh::ApplyMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matLocal)
{
	int size = _indexBones.size();
	int idx = 0;

	D3DXMATRIX m;

	for (int i = 0; i < size; i++)
	{
		idx = _indexBones[i];

		m = *matLocal;// *_boneOffset[idx] * _boneHierarchy->data()[idx]->GetWorldMatrix();
		m *= *matWorld;
		_device->SetTransform(D3DTS_WORLDMATRIX(idx), &m);
	}
}

void SkinnedMesh::LoadVerticeAndIndice(FbxNode& fbxNode, void* pData)
{
	FbxMesh* pMesh = fbxNode.GetMesh();

	std::vector<SKINNEDMESHVERTEX>		bufferVertice;
	std::vector<unsigned long>			bufferIndice;

	int index = 0;
	int polygonCount = pMesh->GetPolygonCount();

	// 버텍스 포지션, 노멀, 텍스쳐 좌표 파싱.
	for (int i = 0; i < polygonCount; i++)
	{
		int numVertices = pMesh->GetPolygonSize(i);

		for (int j = 0; j < numVertices; j++)			// numVertices가 3인지 4일지 알 수가 없다 -> size를 알 수 없다. vector말고 array를 사용하지 못한다.
		{
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);

			SKINNEDMESHVERTEX vertex;
			vertex.Position.x = (float)pMesh->GetControlPointAt(controlPointIndex).mData[0];
			vertex.Position.y = (float)pMesh->GetControlPointAt(controlPointIndex).mData[1];
			vertex.Position.z = (float)pMesh->GetControlPointAt(controlPointIndex).mData[2];

			FbxVector4 normal;
			LoadNormal(*pMesh, controlPointIndex, index, normal);
			vertex.Normal.x = (float)normal.mData[0];
			vertex.Normal.y = (float)normal.mData[1];
			vertex.Normal.z = (float)normal.mData[2];

			FbxVector2 uv;
			LoadTexCoord(*pMesh, controlPointIndex, index, uv);
			vertex.TexCoord.x = (float)uv.mData[0];
			vertex.TexCoord.y = (float)uv.mData[1];

			bufferVertice.push_back(vertex);
			bufferIndice.push_back(index++);
		}
	}

	if (pData)
	{
		_boneHierarchy = static_cast<std::vector<Frame *>*>(pData);
		LoadSkinInfo(*pMesh, bufferVertice, *_boneHierarchy);	// 가중치 및 본 인덱스 파싱.

		std::set<int> indexBone;

		for (int i = 0; i < bufferVertice.size(); i++)
		{
			for (int j = 0; j < 4; j++)
			{
				indexBone.insert(bufferVertice[i].Index[j]);
			}
			
		}

		for (auto it = indexBone.begin(); it != indexBone.end(); ++it)
		{
			_indexBones.push_back(*it);
		}
	}

	const D3DVERTEXELEMENT9 dec[6] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,0 },
		{ 0, 44, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0 },
		D3DDECL_END()
	};

	_vertexCount = bufferVertice.size();
	int bufferVerticeSize = _vertexCount * sizeof(SKINNEDMESHVERTEX);
	if (bufferVerticeSize > 0)					// CreateVertexBuffer일 때 사이즈가 0이면 에러가 뜸.
	{
		_vb->Create(bufferVerticeSize, 0, D3DPOOL_MANAGED);
		_vb->CreateVertexDeclaration(dec);

		void* pData;

		if (_vb->Lock(0, bufferVerticeSize, &pData, 0))
		{
			memcpy(pData, &bufferVertice[0], bufferVerticeSize);

			_vb->Unlock();
		}
	}

	// 16bit 인덱스를 쓸건지 32bit 인덱스를 쓸건지 Device caps를 통해서 알아내기
	_primitiveCount = bufferIndice.size() / 3;
	int bufferIndiceSize = bufferIndice.size() * sizeof(unsigned long);
	if (bufferIndiceSize > 0)
	{
		_ib->Create(bufferIndiceSize, 0, D3DPOOL_MANAGED);
		void * pData;
		if (_ib->Lock(0, bufferIndiceSize, &pData, 0))
		{
			memcpy(pData, &bufferIndice[0], bufferIndiceSize);

			_ib->Unlock();
		}
	}
}


void SkinnedMesh::LoadTexCoord(FbxMesh& fbxMesh, int index, int vertexId, FbxVector2& texCoord)
{

	FbxStringList nameList;
	fbxMesh.GetUVSetNames(nameList);

	const char* name = nameList.GetStringAt(0);
	const FbxGeometryElementUV* uvElement = fbxMesh.GetElementUV(name);

	if (!uvElement) return;

	FbxLayerElement::EMappingMode mapMode = uvElement->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = uvElement->GetReferenceMode();

	switch (mapMode)
	{
	case FbxGeometryElement::eByControlPoint:
		if (refMode == FbxGeometryElement::eDirect)
		{
			texCoord = uvElement->GetDirectArray().GetAt(index);
		}
		else if (refMode == FbxGeometryElement::eIndexToDirect)
		{
			int i = uvElement->GetIndexArray().GetAt(index);
			texCoord = uvElement->GetDirectArray().GetAt(i);
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		if (refMode == FbxGeometryElement::eDirect)
		{
			texCoord = uvElement->GetDirectArray().GetAt(vertexId);
		}
		else if (refMode == FbxGeometryElement::eIndexToDirect)
		{
			int i = uvElement->GetIndexArray().GetAt(vertexId);
			texCoord = uvElement->GetDirectArray().GetAt(i);
		}
		break;
	}
}

void SkinnedMesh::LoadNormal(FbxMesh& fbxMesh, int index, int vertexId, FbxVector4& normal)
{
	FbxGeometryElementNormal* normalElement = fbxMesh.GetElementNormal(0);

	if (!normalElement) return;

	FbxLayerElement::EMappingMode mapMode = normalElement->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = normalElement->GetReferenceMode();
	
	switch (mapMode)
	{
	case FbxGeometryElement::eByControlPoint:
		if (refMode == FbxGeometryElement::eDirect)
		{
			normal = normalElement->GetDirectArray().GetAt(index);
		}
		else if (refMode == FbxGeometryElement::eIndexToDirect)
		{
			int i = normalElement->GetIndexArray().GetAt(index);
			normal = normalElement->GetDirectArray().GetAt(i);
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		if (refMode == FbxGeometryElement::eDirect)
		{
			normal = normalElement->GetDirectArray().GetAt(vertexId);
		}
		else if (refMode == FbxGeometryElement::eIndexToDirect)
		{
			int i = normalElement->GetIndexArray().GetAt(vertexId);
			normal = normalElement->GetDirectArray().GetAt(i);
		}
		break;
	}
}

void SkinnedMesh::LoadSkinInfo(FbxMesh& fbxMesh, std::vector<SKINNEDMESHVERTEX>& bufferVertice, std::vector<Frame *>& boneHierarchy)
{
	int deformerCount = fbxMesh.GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < deformerCount; i++)
	{
		FbxVector4* pIndexedVertex = fbxMesh.GetControlPoints();

		int vertexCount = fbxMesh.GetControlPointsCount();
		std::vector<SKINNEDMESHVERTEX> indexedVertice;

		for (int j = 0; j < vertexCount; j++)
		{
			SKINNEDMESHVERTEX vertex;
			vertex.Position.x = (float)pIndexedVertex[j].mData[0];
			vertex.Position.y = (float)pIndexedVertex[j].mData[1];
			vertex.Position.z = (float)pIndexedVertex[j].mData[2];

			indexedVertice.push_back(vertex);
		}
		
		FbxSkin* skin = FbxCast<FbxSkin>(fbxMesh.GetDeformer(i, FbxDeformer::eSkin));

		if (!skin) continue;

		_boneOffset.resize(boneHierarchy.size());

		int boneCount = skin->GetClusterCount();

		for (int j = 0; j < boneCount; j++)
		{
			FbxCluster* cluster = skin->GetCluster(j);
			FbxNode* bone = cluster->GetLink();
			int boneHierIdx = 0;

			

			for (UINT k = 0; k < boneHierarchy.size(); k++)
			{				
				if (strcmp(bone->GetName(), boneHierarchy[k]->GetName()) == 0)
				{
					boneHierIdx = k;
					break;
				}
			}

			FbxAMatrix LinkBoneMatrix;
			FbxAMatrix TransboneMatrix;
			FbxAMatrix ResultMatrix;

			cluster->GetTransformLinkMatrix(LinkBoneMatrix);
			cluster->GetTransformMatrix(TransboneMatrix);
			ResultMatrix = LinkBoneMatrix.Inverse() * TransboneMatrix;
			
			_boneOffset[boneHierIdx] = FbxDxUtil::ToDX(ResultMatrix);

			double *boneVertexWeights = cluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
			int *boneVertexIndices = cluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들
																				// 해당 본에 영향을 받는 모든 정점을 하나씩 가져옴	
			int numBoneVertexIndices = cluster->GetControlPointIndicesCount();

			for (int boneVertexIndex = 0; boneVertexIndex < numBoneVertexIndices; boneVertexIndex++)
			{
				float tempBoneWeight = (float)boneVertexWeights[boneVertexIndex];	// 영향을 받는 정점의 가중치 정도
																					// 가중치가 0이면 다음걸로 넘김
				if (!tempBoneWeight) continue;

				int tempBoneVertexIndex = boneVertexIndices[boneVertexIndex];			// 영향을 받는 정점의 인덱스

																						// 가중치 중 x가 0이면 첫번째 인덱스
				if (indexedVertice[tempBoneVertexIndex].Weight[0] == 0)
				{
					indexedVertice[tempBoneVertexIndex].Weight[0] = tempBoneWeight;
					indexedVertice[tempBoneVertexIndex].Index[0] = boneHierIdx;
				}
				// 가중치 중 x가 0이 아니고 y가 0이면 두번째 인덱스
				else if (indexedVertice[tempBoneVertexIndex].Weight[0] != 0 &&
						 indexedVertice[tempBoneVertexIndex].Weight[1] == 0)
				{
					indexedVertice[tempBoneVertexIndex].Weight[1] = tempBoneWeight;
					indexedVertice[tempBoneVertexIndex].Index[1] = boneHierIdx;
				}
				// 가중치 중 x가 0이 아니고 y가 0이 아니고 z가 0이면 세번째 인덱스
				else if (indexedVertice[tempBoneVertexIndex].Weight[0] != 0 &&
						 indexedVertice[tempBoneVertexIndex].Weight[1] != 0 &&
						 indexedVertice[tempBoneVertexIndex].Weight[2] == 0)
				{
					indexedVertice[tempBoneVertexIndex].Weight[2] = tempBoneWeight;
					indexedVertice[tempBoneVertexIndex].Index[2] = boneHierIdx;
				}
				// 모두 0이 아니면 4번째 인덱스, 가중치는 1에서 xyz 빼면 나머지 구할 수 있음
				else if (indexedVertice[tempBoneVertexIndex].Weight[0] != 0 &&
						 indexedVertice[tempBoneVertexIndex].Weight[1] != 0 &&
						 indexedVertice[tempBoneVertexIndex].Weight[2] != 0)
				{
					indexedVertice[tempBoneVertexIndex].Index[3] = boneHierIdx;
				}
			}
		}

		for (UINT i = 0; i < bufferVertice.size(); i++)
		{
			if (bufferVertice[i].Index == 0 && bufferVertice[i].Weight[0] == 0)
			{
				SKINNEDMESHVERTEX tempVertex = bufferVertice[i];
				for (UINT j = 0; j < indexedVertice.size(); j++)
				{
					SKINNEDMESHVERTEX tempVertex2 = indexedVertice[j];

					if (tempVertex.Position == tempVertex2.Position)
					{
						bufferVertice[i].Weight[0] = indexedVertice[j].Weight[0];
						bufferVertice[i].Weight[1] = indexedVertice[j].Weight[1];
						bufferVertice[i].Weight[2] = indexedVertice[j].Weight[2];

						bufferVertice[i].Index[0] = indexedVertice[j].Index[0];
						bufferVertice[i].Index[1] = indexedVertice[j].Index[1];
						bufferVertice[i].Index[2] = indexedVertice[j].Index[2];
						bufferVertice[i].Index[3] = indexedVertice[j].Index[3];
						break;
					}
				}
			}
		}
	}
}