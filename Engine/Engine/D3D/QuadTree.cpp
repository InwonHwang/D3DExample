#include "QuadTree.h"
#include "Frustum.h"
#include "Description\D3DDescription.h"

QuadTree::QuadTree(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight)
	:
	_parent(parent),
	_radius(0),
	_culled(false)
{
	for (int i = 0; i < 4; i++)
	{
		_child[i] = nullptr;
		_neighbour[i] = nullptr;
	}

	SetConers(topLeft, topRight, bottomLeft, bottomRight);
}

QuadTree::QuadTree(int x, int z)
	: QuadTree(nullptr, 0, x - 1, x * (z - 1), x * z - 1)
{
}

QuadTree::~QuadTree()
{
	Destroy();
}
////////////////////////////////////////////////////////////////////////////////////////
//////////////// public method /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

bool QuadTree::Build(sp<HeightMapVec> pHeightMapVec)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	BuildQuadTree(pHeightMapVec);
	BuildNeighbourNode(this, pHeightMapVec, _coner[1] + 1);
	return true;
}

int QuadTree::GenerateIndex(void* indices, sp<HeightMapVec> pHeightMapVec, sp<Frustum> pFrustum, float LODRatio)
{
	assert(indices && "null reference: indices");
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	ProcessFrustumCulling(pHeightMapVec, pFrustum);
	return GenrateTriangleIndex(0, indices, pHeightMapVec, pFrustum, LODRatio);
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////// private method ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/// constructor
void QuadTree::SetConers(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	_coner[0] = topLeft;
	_coner[1] = topRight;
	_coner[2] = bottomLeft;
	_coner[3] = bottomRight;
	_center = (_coner[0] + _coner[1] + _coner[2] + _coner[3]) / 4;
}

/// destructor
void QuadTree::Destroy()
{
	SafeDelete<QuadTree>(_child[0]);
	SafeDelete<QuadTree>(_child[1]);
	SafeDelete<QuadTree>(_child[2]);
	SafeDelete<QuadTree>(_child[3]);
}

/// build
bool QuadTree::BuildQuadTree(sp<HeightMapVec> pHeightMapVec)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");

	if (SubDivide())
	{
		// 좌측상단과, 우측 하단의 거리를 구한다.
		Vector3 v = pHeightMapVec->data()[_coner[0]] - pHeightMapVec->data()[_coner[3]];
		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 
		// 2로 나누어 반지름을 구한다.
		_radius = D3DXVec3Length(&v) / 2.0f;
		_child[0]->BuildQuadTree(pHeightMapVec);
		_child[1]->BuildQuadTree(pHeightMapVec);
		_child[2]->BuildQuadTree(pHeightMapVec);
		_child[3]->BuildQuadTree(pHeightMapVec);
	}
	return true;
}

void QuadTree::BuildNeighbourNode(QuadTree* pRoot, sp<HeightMapVec> pHeightMapVec, int width)
{
	assert(pRoot && "null reference: pRoot");
	assert(pHeightMapVec && "null reference: pHeightMapVec");

	bool isNeighbourVaild;

	int topLeft;
	int topRight;
	int bottomLeft;
	int bottomRight;

	for (int i = 0; i < 4; i++)
	{
		// 이웃노드의 4개 코너값을 얻는다.
		isNeighbourVaild = GetNeighbourConers(i, width, topLeft, topRight, bottomLeft, bottomRight);
		// 코너값으로 이웃노드의 포인터를 얻어온다.

		if (isNeighbourVaild)
		{
			_neighbour[i] = pRoot->FindNode(pHeightMapVec, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	// 자식노드로 재귀호출
	if (_child[0])
	{
		_child[0]->BuildNeighbourNode(pRoot, pHeightMapVec, width);
		_child[1]->BuildNeighbourNode(pRoot, pHeightMapVec, width);
		_child[2]->BuildNeighbourNode(pRoot, pHeightMapVec, width);
		_child[3]->BuildNeighbourNode(pRoot, pHeightMapVec, width);
	}
}

///generateIndex
void QuadTree::ProcessFrustumCulling(sp<HeightMapVec> pHeightMapVec, sp<Frustum> pFrustum)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	int isFrustum = IsInFrustum(pHeightMapVec, pFrustum);

	switch (isFrustum)
	{
	case FRUSTUM_OUT:				// 프러스텀에서 완전벗어남, 하위노드 검색 필요없음
		_culled = true;
		return;
	case FRUSTUM_IN:	// 프러스텀에 완전포함, 하위노드 검색 필요없음
		SetInFrustumAll();
		return;
	case FRUSTUM_PARTIALLY_IN:		// 프러스텀에 일부포함, 하위노드 검색 필요함
		_culled = false;
		if (_child[0]) _child[0]->ProcessFrustumCulling(pHeightMapVec, pFrustum);
		if (_child[1]) _child[1]->ProcessFrustumCulling(pHeightMapVec, pFrustum);
		if (_child[2]) _child[2]->ProcessFrustumCulling(pHeightMapVec, pFrustum);
		if (_child[3]) _child[3]->ProcessFrustumCulling(pHeightMapVec, pFrustum);
	}
}



int QuadTree::GenrateTriangleIndex(int numTriangles, void* indices, sp<HeightMapVec> pHeightMapVec, const sp<Frustum> pFrustum, float LODRatio)
{
	assert(indices && "null reference: indices");
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	if (_culled)
	{
		_culled = false;
		return numTriangles;
	}

	if (IsVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio))
	{
		INDEX* p = ((INDEX*)indices) + numTriangles;
		INDEX index;

		if (_coner[1] - _coner[0] <= 1)
		{
			index._0 = _coner[0];
			index._1 = _coner[2];
			index._2 = _coner[1];
			
			*p++ = index;
			numTriangles++;

			index._0 = _coner[2];
			index._1 = _coner[3];
			index._2 = _coner[1];
			
			*p++ = index;
			numTriangles++;

			return numTriangles;
		}

		bool b[5] = { false, };
		
		if (_neighbour[UP]) b[UP] = _neighbour[UP]->IsVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);
		if (_neighbour[DN]) b[DN] = _neighbour[DN]->IsVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);
		if (_neighbour[LT]) b[LT] = _neighbour[LT]->IsVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);
		if (_neighbour[RT]) b[RT] = _neighbour[RT]->IsVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);

		b[4] = b[UP] & b[DN] & b[LT] & b[RT];		
		
		if (b[4])
		{	
			index._0 = _coner[0];
			index._1 = _coner[2];
			index._2 = _coner[1];
			
			*p++ = index;
			numTriangles++;

			index._0 = _coner[2];
			index._1 = _coner[3];
			index._2 = _coner[1];			
			*p++ = index;
			numTriangles++;
			return numTriangles;
		}

		int	n;

		if (!b[UP]) // 상단 부분분할이 필요한가?
		{			
			n = (_coner[2] + _coner[3]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _coner[3];
			*p++ = index;
			numTriangles++;

			index._0 = _center;
			index._1 = _coner[2];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// 상단 부분분할이 필요없을 경우
		{
			index._0 = _center;
			index._1 = _coner[2];
			index._2 = _coner[3];
			*p++ = index;
			numTriangles++;
		}

		if (!b[DN]) // 하단 부분분할이 필요한가?
		{
			n = (_coner[0] + _coner[1]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _coner[0];
			*p++ = index;
			numTriangles++;
			index._0 = _center;
			index._1 = _coner[1];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// 하단 부분분할이 필요없을 경우
		{
			index._0 = _center;
			index._1 = _coner[1];
			index._2 = _coner[0];
			*p++ = index;
			numTriangles++;
		}

		if (!b[LT]) // 좌측 부분분할이 필요한가?
		{
			n = (_coner[0] + _coner[2]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _coner[2];			
			*p++ = index;
			numTriangles++;
			index._0 = _center;
			index._1 = _coner[0];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// 좌측 부분분할이 필요없을 경우
		{
			index._0 = _center;
			index._1 = _coner[0];
			index._2 = _coner[2];			
			*p++ = index;
			numTriangles++;
		}

		if (!b[RT]) // 우측 부분분할이 필요한가?
		{
			n = (_coner[1] + _coner[3]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _coner[1];			
			*p++ = index;
			numTriangles++;

			index._0 = _center;
			index._1 = _coner[3];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// 우측 부분분할이 필요없을 경우
		{
			index._0 = _center;
			index._1 = _coner[3];
			index._2 = _coner[1];			
			*p++ = index;
			numTriangles++;
		}

		return numTriangles;	// 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!
	}

	if (_child[0]) numTriangles = _child[0]->GenrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);
	if (_child[1]) numTriangles = _child[1]->GenrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);
	if (_child[2]) numTriangles = _child[2]->GenrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);
	if (_child[3]) numTriangles = _child[3]->GenrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);

	return numTriangles;
}

/// buildQuadTree
bool QuadTree::SubDivide()
{
	if ((_coner[1] - _coner[0]) <= 1) return false;

	int topEdgeCenter = (_coner[0] + _coner[1]) / 2;
	int bottomEdgeCenter = (_coner[2] + _coner[3]) / 2;
	int leftEdgeCenter = (_coner[0] + _coner[2]) / 2;
	int rightEdgeCenter = (_coner[1] + _coner[3]) / 2;
	int centerPoint = (_coner[0] + _coner[1] + _coner[2] + _coner[3]) / 4;

	_child[0] = CreateChild(this, _coner[0], topEdgeCenter, leftEdgeCenter, centerPoint);
	_child[1] = CreateChild(this, topEdgeCenter, _coner[1], centerPoint, rightEdgeCenter);
	_child[2] = CreateChild(this, leftEdgeCenter, centerPoint, _coner[2], bottomEdgeCenter);
	_child[3] = CreateChild(this, centerPoint, rightEdgeCenter, bottomEdgeCenter, _coner[3]);

	return true;
}

/// buildNeighbourNode
QuadTree* QuadTree::FindNode(sp<HeightMapVec> pHeightMapVec, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");

	if (_coner[0] == topLeft &&
		_coner[1] == topRight &&
		_coner[2] == bottomLeft &&
		_coner[3] == bottomRight)
	{
		return this;
	}
	

	if (_child[0])
	{
		RECT	rc;
		
		POINT	pt;
		int n = (topLeft + topRight + bottomLeft + bottomRight) / 4;

		// 현재 맵상에서의 위치
		pt.x = (int)pHeightMapVec->data()[n].x;
		pt.y = (int)pHeightMapVec->data()[n].z;

		for (int i = 0; i < 4; i++)
		{
			SetRect(&rc, (int)pHeightMapVec->data()[_child[i]->_coner[2]].x,
						(int)pHeightMapVec->data()[_child[i]->_coner[2]].z,
						(int)pHeightMapVec->data()[_child[i]->_coner[1]].x,
						(int)pHeightMapVec->data()[_child[i]->_coner[1]].z);
			// pt값이 점유범위안에 있다면 자식노드로 들어간다.
			if ((rc.left <= pt.x) && (pt.x <= rc.right) && (rc.bottom <= pt.y) && (pt.y <= rc.top))
				return _child[i]->FindNode(pHeightMapVec, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	return nullptr;
}

bool QuadTree::GetNeighbourConers(int edgeType, int width, int& bottomLeft, int& bottomRight, int& topLeft, int& topRight)
{
	int edgeSize = _coner[1] - _coner[0];	// 현재 노드의 좌우폭값

	switch (edgeType)
	{
	case UP:	// 위쪽 방향 이웃노드의 인덱스
		topLeft = _coner[2] + width * edgeSize;
		topRight = _coner[3] + width * edgeSize;
		bottomLeft = _coner[2];
		bottomRight = _coner[3];
		break;
	case DN:	// 아래 방향 이웃노드의 인덱스
		topLeft = _coner[0];
		topRight = _coner[1];
		bottomLeft = _coner[0] - width * edgeSize;
		bottomRight = _coner[1] - width * edgeSize;
		break;
	case LT:	// 좌측 방향 이웃노드의 인덱스
		topLeft = _coner[2] - edgeSize;
		topRight = _coner[2];
		bottomLeft = _coner[0] - edgeSize;
		bottomRight = _coner[0];
		break;
	case RT:	// 우측 방향 이웃노드의 인덱스
		topLeft = _coner[3];
		topRight = _coner[3] + edgeSize;
		bottomLeft = _coner[1];
		bottomRight = _coner[1] + edgeSize;
		break;
	}

	bool b[4] = { false, };
	b[0] = topRight > width * width;		// 위쪽 이웃노드가 HeightMap 범위를 벗어남.
	b[1] = bottomLeft < 0;					// 아래쪽 이웃노드가 HeightMap 범위를 벗어남.
	b[2] = topLeft % width == width - 1;	// 왼쪽 이웃노드가 HeightMap 범위를 벗어남.
	b[3] = topRight % width == 0;			// 오른쪽 이웃노드가 HeightMap 범위를 벗어남.

	if (b[0] | b[1] | b[2] | b[3])
		return false;

	return true;
}

/// processFrustumCulling
void	QuadTree::SetInFrustumAll()
{
	_culled = false;
	if (!_child[0]) return;
	_child[0]->SetInFrustumAll();
	_child[1]->SetInFrustumAll();
	_child[2]->SetInFrustumAll();
	_child[3]->SetInFrustumAll();
}



int QuadTree::IsInFrustum(sp<HeightMapVec> pHeightMapVec, sp<Frustum> pFrustum) const
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	if (!pFrustum->IsInSphere(pHeightMapVec->data()[_center], _radius)) return FRUSTUM_OUT;

	if (pFrustum->IsIn(pHeightMapVec->data()[_coner[0]]) &&
		pFrustum->IsIn(pHeightMapVec->data()[_coner[1]]) &&
		pFrustum->IsIn(pHeightMapVec->data()[_coner[2]]) &&
		pFrustum->IsIn(pHeightMapVec->data()[_coner[3]])) return FRUSTUM_IN;

	return FRUSTUM_PARTIALLY_IN;
}




