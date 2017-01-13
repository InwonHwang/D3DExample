#include "StaticMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

StaticMesh::StaticMesh(IDirect3DDevice9& device)
	: Mesh(device)
{	
	_isSkinned = false;
	_vb = new VertexBuffer(device);
	_ib = new IndexBuffer(device);	
}


StaticMesh::~StaticMesh()
{	
	SAFE_DELETE(_vb);
	SAFE_DELETE(_ib);
}


// inherited from Mesh
void StaticMesh::Draw()
{
	assert(_vb->GetVertexBuffer() && "vertex buffer doesn't created!!");
	assert(_ib->GetData() && "index buffer doesn't created!!");

	_device->SetStreamSource(0, _vb->GetVertexBuffer(), 0, sizeof(STATICMESHVERTEX));
	_device->SetVertexDeclaration(_vb->GetVertexDeclaration());
	_device->SetIndices(_ib->GetData());
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertexCount, 0, _primitiveCount);

}

// inherited from IMemento
void StaticMesh::Load(FbxNode& fbxNode, void* pData)
{
	FbxMesh* pMesh = fbxNode.GetMesh();

	if (pMesh == nullptr) return;

	std::vector<STATICMESHVERTEX>	bufferVertex;
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

			STATICMESHVERTEX vertex;

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

	// 정점 버퍼 정보 채우기
	_vertexCount = bufferVertex.size();	
	int bufferVertexSize = _vertexCount * sizeof(STATICMESHVERTEX);

	if (!_device)
		DebugBox(0, 0);

	if (bufferVertexSize > 0)	// CreateVertexBuffer일 때 사이즈가 0이면 에러가 뜸.
	{
		_vb->Create(bufferVertexSize, 0, D3DPOOL_MANAGED);
		_vb->CreateVertexDeclaration(StaticMeshDec);
	
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

void StaticMesh::Save(FbxNode& fbxNode)
{

}

void StaticMesh::LoadPosition(FbxMesh& mesh, int ctrlPointIndex, D3DXVECTOR3& position)
{
	FbxVector4* pCtrlPoint = mesh.GetControlPoints();

	position.x = (float)pCtrlPoint[ctrlPointIndex].mData[0];
	position.y = (float)pCtrlPoint[ctrlPointIndex].mData[1];
	position.z = (float)pCtrlPoint[ctrlPointIndex].mData[2];
}

void StaticMesh::LoadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR4& color)
{
	if (mesh.GetElementVertexColorCount() < 1) return;

	FbxGeometryElementVertexColor* pVertexColor = mesh.GetElementVertexColor(0);

	FbxLayerElement::EMappingMode mappingMode = pVertexColor->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pVertexColor->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint :
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect :
			{
				color.x = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
				color.y = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
				color.z = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
				color.w = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
			}
			break;
		case FbxGeometryElement::eIndexToDirect :
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
	case FbxGeometryElement::eByPolygonVertex :
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
void StaticMesh::LoadUV(FbxMesh& mesh, int ctrlPointIndex, int uvIndex, D3DXVECTOR2& uv)
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
void StaticMesh::LoadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& normal)
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
void StaticMesh::LoadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& tangent)
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
