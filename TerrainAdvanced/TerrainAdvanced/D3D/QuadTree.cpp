#include "QuadTree.h"
#include "Frustum.h"


QuadTree::QuadTree(int topLeft, int topRight, int bottomLeft, int bottomRight)
	:
	_radius(0),
	_culled(false)
{	
	_child[0] = nullptr;
	_child[1] = nullptr;
	_child[2] = nullptr;
	_child[3] = nullptr;

	setConers(topLeft, topRight, bottomLeft, bottomRight);
}

QuadTree::QuadTree(int x, int z)
	: QuadTree(0, x - 1, x * (z - 1), x * z - 1)
{
}

QuadTree::~QuadTree()
{
	destroy();
}

void QuadTree::destroy()
{
	SAFE_DELETE(_child[0]);
	SAFE_DELETE(_child[1]);
	SAFE_DELETE(_child[2]);
	SAFE_DELETE(_child[3]);
}

bool QuadTree::build(Vector3* heightMap)
{
	if (subDivide())
	{
		// 좌측상단과, 우측 하단의 거리를 구한다.
		D3DXVECTOR3 v = *(heightMap + _coner[TOP_LEFT]) - *(heightMap + _coner[BOTTOM_RIGHT]);

		_radius = D3DXVec3Length(&v) / 2.0f;		
		_child[TOP_LEFT]->build(heightMap);
		_child[TOP_RIGHT]->build(heightMap);
		_child[BOTTOM_LEFT]->build(heightMap);
		_child[BOTTOM_RIGHT]->build(heightMap);
	}
	return true;
}

int QuadTree::generateIndex(void* indices, Vector3* heightMap, Frustum& f, float LODRatio)
{
	processFrustumCulling(heightMap, f);
	return genrateTriangleIndex(0, indices, heightMap, f, LODRatio);
}

bool QuadTree::subDivide()
{
	if ((_coner[TOP_RIGHT] - _coner[TOP_LEFT]) <= 1) return false;

	int topEdgeCenter = (_coner[TOP_LEFT] + _coner[TOP_RIGHT]) / 2;
	int bottomEdgeCenter = (_coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 2;
	int leftEdgeCenter = (_coner[TOP_LEFT] + _coner[BOTTOM_LEFT]) / 2;
	int rightEdgeCenter = (_coner[TOP_RIGHT] + _coner[BOTTOM_RIGHT]) / 2;
	int centerPoint = (_coner[TOP_LEFT] + _coner[TOP_RIGHT] + _coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 4;


	_child[TOP_LEFT] = createChild(_coner[TOP_LEFT], topEdgeCenter, leftEdgeCenter, centerPoint);
	_child[TOP_RIGHT] = createChild(topEdgeCenter, _coner[TOP_RIGHT], centerPoint, rightEdgeCenter);
	_child[BOTTOM_LEFT] = createChild(leftEdgeCenter, centerPoint, _coner[BOTTOM_LEFT], bottomEdgeCenter);
	_child[BOTTOM_RIGHT] = createChild(centerPoint, rightEdgeCenter, bottomEdgeCenter, _coner[BOTTOM_RIGHT]);

	return true;
}

void QuadTree::setConers(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	_coner[TOP_LEFT] = topLeft;
	_coner[TOP_RIGHT] = topRight;
	_coner[BOTTOM_LEFT] = bottomLeft;
	_coner[BOTTOM_RIGHT] = bottomRight;
	_center = (_coner[TOP_LEFT] + _coner[TOP_RIGHT] + _coner[BOTTOM_LEFT] + _coner[BOTTOM_RIGHT]) / 4;
}

void QuadTree::processFrustumCulling(Vector3* heightMap, Frustum& frustum)
{
	int isFrustum = isInFrustum(heightMap, frustum);

	switch (isFrustum)
	{
	case FRUSTUM_OUT:				// 프러스텀에서 완전벗어남, 하위노드 검색 필요없음
		_culled = true;
		return;
	case FRUSTUM_IN:	// 프러스텀에 완전포함, 하위노드 검색 필요없음
		_culled = false;
		return;
	case FRUSTUM_PARTIALLY_IN:		// 프러스텀에 일부포함, 하위노드 검색 필요함
		_culled = false;
		if (_child[TOP_LEFT]) _child[TOP_LEFT]->processFrustumCulling(heightMap, frustum);
		if (_child[TOP_RIGHT]) _child[TOP_RIGHT]->processFrustumCulling(heightMap, frustum);
		if (_child[BOTTOM_LEFT]) _child[BOTTOM_LEFT]->processFrustumCulling(heightMap, frustum);
		if (_child[BOTTOM_RIGHT]) _child[BOTTOM_RIGHT]->processFrustumCulling(heightMap, frustum);
	}
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

	if (_child[TOP_LEFT]) numTriangles = _child[TOP_LEFT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);
	if (_child[TOP_RIGHT]) numTriangles = _child[TOP_RIGHT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);
	if (_child[BOTTOM_LEFT]) numTriangles = _child[BOTTOM_LEFT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);
	if (_child[BOTTOM_RIGHT]) numTriangles = _child[BOTTOM_RIGHT]->genrateTriangleIndex(numTriangles, indices, heightMap, f, LODRatio);

	return numTriangles;
}