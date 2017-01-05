#include "QuadTree.h"
#include "Frustum.h"


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

	setConers(topLeft, topRight, bottomLeft, bottomRight);
}

QuadTree::QuadTree(int x, int z)
	: QuadTree(nullptr, 0, x - 1, x * (z - 1), x * z - 1)
{
}

QuadTree::~QuadTree()
{
	destroy();
}
////////////////////////////////////////////////////////////////////////////////////////
//////////////// public method /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

bool QuadTree::build(Vector3* heightMap)
{
	buildQuadTree(heightMap);
	buildNeighbourNode(this, heightMap, _coner[TOP_RIGHT] + 1);
	return true;
}

int QuadTree::generateIndex(void* indices, Vector3* heightMap, Frustum& f, float LODRatio)
{
	processFrustumCulling(heightMap, f);
	return genrateTriangleIndex(0, indices, heightMap, f, LODRatio);
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////// private method ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/// constructor
void QuadTree::setConers(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	_coner[TOP_LEFT] = topLeft;
	_coner[TOP_RIGHT] = topRight;
	_coner[BOTTOM_LEFT] = bottomLeft;
	_coner[BOTTOM_RIGHT] = bottomRight;
	_center = (_coner[TOP_LEFT] + _coner[TOP_RIGHT] + _coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 4;
}

/// destructor
void QuadTree::destroy()
{
	SAFE_DELETE(_child[0]);
	SAFE_DELETE(_child[1]);
	SAFE_DELETE(_child[2]);
	SAFE_DELETE(_child[3]);
}

/// build
bool QuadTree::buildQuadTree(Vector3* heightMap)
{
	if (subDivide())
	{
		// 좌측상단과, 우측 하단의 거리를 구한다.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(heightMap + _coner[TOP_LEFT])) -
			*((D3DXVECTOR3*)(heightMap + _coner[BOTTOM_RIGHT]));
		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 
		// 2로 나누어 반지름을 구한다.
		_radius = D3DXVec3Length(&v) / 2.0f;
		_child[TOP_LEFT]->buildQuadTree(heightMap);
		_child[TOP_RIGHT]->buildQuadTree(heightMap);
		_child[BOTTOM_LEFT]->buildQuadTree(heightMap);
		_child[BOTTOM_RIGHT]->buildQuadTree(heightMap);
	}
	return true;
}

void QuadTree::buildNeighbourNode(QuadTree* pRoot, Vector3* heightMap, int cx)
{
	int				n;
	int				topLeft, topRight, bottomLeft, bottomRight;

	for (int i = 0; i < 4; i++)
	{
		topLeft = _coner[0];
		topRight = _coner[1];
		bottomLeft = _coner[2];
		bottomRight = _coner[3];
		// 이웃노드의 4개 코너값을 얻는다.
		n = getNodeIndex(i, cx, topLeft, topRight, bottomLeft, bottomRight);
		// 코너값으로 이웃노드의 포인터를 얻어온다.
		if (n > 0) _neighbour[i] = pRoot->findNode(heightMap, topLeft, topRight, bottomLeft, bottomRight);
	}

	// 자식노드로 재귀호출
	if (_child[0])
	{
		_child[0]->buildNeighbourNode(pRoot, heightMap, cx);
		_child[1]->buildNeighbourNode(pRoot, heightMap, cx);
		_child[2]->buildNeighbourNode(pRoot, heightMap, cx);
		_child[3]->buildNeighbourNode(pRoot, heightMap, cx);
	}
}

///generateIndex
void QuadTree::processFrustumCulling(Vector3* heightMap, Frustum& frustum)
{
	int isFrustum = isInFrustum(heightMap, frustum);

	switch (isFrustum)
	{
	case FRUSTUM_OUT:				// 프러스텀에서 완전벗어남, 하위노드 검색 필요없음
		_culled = true;
		return;
	case FRUSTUM_IN:	// 프러스텀에 완전포함, 하위노드 검색 필요없음
		setInFrustumAll();
		return;
	case FRUSTUM_PARTIALLY_IN:		// 프러스텀에 일부포함, 하위노드 검색 필요함
		_culled = false;
		if (_child[TOP_LEFT]) _child[TOP_LEFT]->processFrustumCulling(heightMap, frustum);
		if (_child[TOP_RIGHT]) _child[TOP_RIGHT]->processFrustumCulling(heightMap, frustum);
		if (_child[BOTTOM_LEFT]) _child[BOTTOM_LEFT]->processFrustumCulling(heightMap, frustum);
		if (_child[BOTTOM_RIGHT]) _child[BOTTOM_RIGHT]->processFrustumCulling(heightMap, frustum);
	}
}



int QuadTree::genrateTriangleIndex(int numTriangles, void* indices, Vector3* heightMap, const Frustum& f, float LODRatio)
{
	if (_culled)
	{
		_culled = false;
		return numTriangles;
	}

	if (isVisible(heightMap, f.getPos(), LODRatio))
	{
#ifdef _USE_INDEX16
		LPWORD p = ((LPWORD)indices) + numTriangles * 3;
#else
		LPDWORD p = ((LPDWORD)indices) + numTriangles * 3;
#endif
		if (_coner[TOP_RIGHT] - _coner[TOP_LEFT] <= 1)
		{
			*p++ = _coner[TOP_LEFT];
			*p++ = _coner[TOP_RIGHT];
			*p++ = _coner[BOTTOM_LEFT];
			numTriangles++;

			*p++ = _coner[BOTTOM_LEFT];
			*p++ = _coner[TOP_RIGHT];
			*p++ = _coner[BOTTOM_RIGHT];
			numTriangles++;
			return numTriangles;
		}

		bool	b[4] = { false, };

		if (_neighbour[EDGE_UP]) b[EDGE_UP] = _neighbour[EDGE_UP]->isVisible(heightMap, f.getPos(), LODRatio);
		if (_neighbour[EDGE_DN]) b[EDGE_DN] = _neighbour[EDGE_DN]->isVisible(heightMap, f.getPos(), LODRatio);
		if (_neighbour[EDGE_LT]) b[EDGE_LT] = _neighbour[EDGE_LT]->isVisible(heightMap, f.getPos(), LODRatio);
		if (_neighbour[EDGE_RT]) b[EDGE_RT] = _neighbour[EDGE_RT]->isVisible(heightMap, f.getPos(), LODRatio);

		if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
		{
			*p++ = _coner[0];
			*p++ = _coner[1];
			*p++ = _coner[2];
			numTriangles++;
			*p++ = _coner[2];
			*p++ = _coner[1];
			*p++ = _coner[3];
			numTriangles++;
			return numTriangles;
		}

		int		n;

		if (!b[EDGE_UP]) // 상단 부분분할이 필요한가?
		{
			n = (_coner[TOP_LEFT] + _coner[TOP_RIGHT]) / 2;
			*p++ = _center;
			*p++ = _coner[TOP_LEFT];
			*p++ = n;
			numTriangles++;
			*p++ = _center;
			*p++ = n;
			*p++ = _coner[TOP_RIGHT];
			numTriangles++;
		}
		else	// 상단 부분분할이 필요없을 경우
		{
			*p++ = _center;
			*p++ = _coner[TOP_LEFT];
			*p++ = _coner[TOP_RIGHT];
			numTriangles++;
		}

		if (!b[EDGE_DN]) // 하단 부분분할이 필요한가?
		{
			n = (_coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 2;
			*p++ = _center;
			*p++ = _coner[BOTTOM_RIGHT];
			*p++ = n;
			numTriangles++;
			*p++ = _center;
			*p++ = n;
			*p++ = _coner[BOTTOM_LEFT];
			numTriangles++;
		}
		else	// 하단 부분분할이 필요없을 경우
		{
			*p++ = _center;
			*p++ = _coner[BOTTOM_RIGHT];
			*p++ = _coner[BOTTOM_LEFT];
			numTriangles++;
		}

		if (!b[EDGE_LT]) // 좌측 부분분할이 필요한가?
		{
			n = (_coner[TOP_LEFT] + _coner[BOTTOM_LEFT]) / 2;
			*p++ = _center;
			*p++ = _coner[BOTTOM_LEFT];
			*p++ = n;
			numTriangles++;
			*p++ = _center;
			*p++ = n; *p++ = _coner[TOP_LEFT];
			numTriangles++;
		}
		else	// 좌측 부분분할이 필요없을 경우
		{
			*p++ = _center;
			*p++ = _coner[BOTTOM_LEFT];
			*p++ = _coner[TOP_LEFT];
			numTriangles++;
		}

		if (!b[EDGE_RT]) // 우측 부분분할이 필요한가?
		{
			n = (_coner[TOP_RIGHT] + _coner[BOTTOM_RIGHT]) / 2;
			*p++ = _center;
			*p++ = _coner[TOP_RIGHT];
			*p++ = n;
			numTriangles++;
			*p++ = _center;
			*p++ = n;
			*p++ = _coner[BOTTOM_RIGHT];
			numTriangles++;
		}
		else	// 우측 부분분할이 필요없을 경우
		{
			*p++ = _center;
			*p++ = _coner[TOP_RIGHT];
			*p++ = _coner[BOTTOM_RIGHT];
			numTriangles++;
		}

		return numTriangles;	// 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!
	}

	if (_child[TOP_LEFT]) numTriangles = _child[TOP_LEFT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);
	if (_child[TOP_RIGHT]) numTriangles = _child[TOP_RIGHT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);
	if (_child[BOTTOM_LEFT]) numTriangles = _child[BOTTOM_LEFT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);
	if (_child[BOTTOM_RIGHT]) numTriangles = _child[BOTTOM_RIGHT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);

	return numTriangles;
}

/// buildQuadTree
bool QuadTree::subDivide()
{
	if ((_coner[TOP_RIGHT] - _coner[TOP_LEFT]) <= 1) return false;

	int topEdgeCenter = (_coner[TOP_LEFT] + _coner[TOP_RIGHT]) / 2;
	int bottomEdgeCenter = (_coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 2;
	int leftEdgeCenter = (_coner[TOP_LEFT] + _coner[BOTTOM_LEFT]) / 2;
	int rightEdgeCenter = (_coner[TOP_RIGHT] + _coner[BOTTOM_RIGHT]) / 2;
	int centerPoint = (_coner[TOP_LEFT] + _coner[TOP_RIGHT] + _coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 4;


	_child[TOP_LEFT] = createChild(this, _coner[TOP_LEFT], topEdgeCenter, leftEdgeCenter, centerPoint);
	_child[TOP_RIGHT] = createChild(this, topEdgeCenter, _coner[TOP_RIGHT], centerPoint, rightEdgeCenter);
	_child[BOTTOM_LEFT] = createChild(this, leftEdgeCenter, centerPoint, _coner[BOTTOM_LEFT], bottomEdgeCenter);
	_child[BOTTOM_RIGHT] = createChild(this, centerPoint, rightEdgeCenter, bottomEdgeCenter, _coner[BOTTOM_RIGHT]);

	return true;
}

/// buildNeighbourNode
QuadTree* QuadTree::findNode(Vector3* heightMap, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	if ((_coner[0] == topLeft) && (_coner[1] == topRight) && (_coner[2] == bottomLeft) && (_coner[3] == bottomRight))
		return this;

	if (_child[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (topLeft + topRight + bottomLeft + bottomRight) / 4;

		// 현재 맵상에서의 위치
		pt.x = (int)heightMap[n].x;
		pt.y = (int)heightMap[n].z;

		for (int i = 0; i < 4; i++)
		{
			SetRect(&rc, (int)heightMap[_child[i]->_coner[TOP_LEFT]].x,
				(int)heightMap[_child[i]->_coner[TOP_LEFT]].z,
				(int)heightMap[_child[i]->_coner[BOTTOM_RIGHT]].x,
				(int)heightMap[_child[i]->_coner[BOTTOM_RIGHT]].z);
			// pt값이 점유범위안에 있다면 자식노드로 들어간다.
			if (IsInRect(&rc, pt))
				return _child[i]->findNode(heightMap, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	return nullptr;
}

int	QuadTree::getNodeIndex(int ed, int cx, int& topLeft, int& topRight, int& bottomLeft, int& bottomRight)
{
	int		n, gap;
	int coner[4];

	coner[0] = topLeft;
	coner[1] = topRight;
	coner[2] = bottomLeft;
	coner[3] = bottomRight;
	gap = coner[1] - coner[0];	// 현재 노드의 좌우폭값

	switch (ed)
	{
	case EDGE_UP:	// 위쪽 방향 이웃노드의 인덱스
		topLeft = coner[0] - cx * gap;
		topRight = coner[1] - cx * gap;
		bottomLeft = coner[0];
		bottomRight = coner[1];
		break;
	case EDGE_DN:	// 아래 방향 이웃노드의 인덱스
		topLeft = coner[2];
		topRight = coner[3];
		bottomLeft = coner[2] + cx * gap;
		bottomRight = coner[3] + cx * gap;
		break;
	case EDGE_LT:	// 좌측 방향 이웃노드의 인덱스
		topLeft = coner[0] - gap;
		topRight = coner[0];
		bottomLeft = coner[2] - gap;
		bottomRight = coner[2];
		break;
	case EDGE_RT:	// 우측 방향 이웃노드의 인덱스
		topLeft = coner[1];
		topRight = coner[1] + gap;
		bottomLeft = coner[3];
		bottomRight = coner[3] + gap;
		if ((topLeft / cx) != (coner[0] / cx)) return -1;
		if ((topRight / cx) != (coner[1] / cx)) return -1;
		break;
	}

	n = (topLeft + topRight + bottomLeft + bottomRight) / 4;	// 가운데 인덱스
	if (!(n >= 0 && n <= cx * cx - 1)) return -1;

	return n;
}

/// processFrustumCulling
void	QuadTree::setInFrustumAll()
{
	_culled = false;
	if (!_child[0]) return;
	_child[0]->setInFrustumAll();
	_child[1]->setInFrustumAll();
	_child[2]->setInFrustumAll();
	_child[3]->setInFrustumAll();
}



int QuadTree::isInFrustum(Vector3* heightMap, Frustum& f) const
{
	if (!f.isInSphere(*(heightMap + _center), _radius)) return FRUSTUM_OUT;

	if (f.isIn(*(heightMap + _coner[TOP_LEFT])) &&
		f.isIn(*(heightMap + _coner[TOP_RIGHT])) &&
		f.isIn(*(heightMap + _coner[BOTTOM_LEFT])) &&
		f.isIn(*(heightMap + _coner[BOTTOM_RIGHT]))) return FRUSTUM_IN;

	return FRUSTUM_PARTIALLY_IN;
}




