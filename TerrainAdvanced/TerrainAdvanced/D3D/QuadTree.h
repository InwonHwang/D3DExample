#pragma once

#include "Math\Math.h"
#include "../Core/Core.h"
#include "Description\Description.h"

class Frustum;

class QuadTree
{
	enum ConerType
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT	
		
	};

	enum State
	{
		FRUSTUM_OUT,
		FRUSTUM_PARTIALLY_IN,
		FRUSTUM_IN
	};
public:
	QuadTree(int topLeft, int topRight, int bottomLeft, int bottomRight);
	QuadTree(int cx, int cy);	
	~QuadTree();

	void destroy();
	bool build(Vector3* heightMap);

	int generateIndex(void* indices, Vector3* heightMap, Frustum& f, float LODRatio);

private:	
	bool subDivide();
	void setConers(int topLeft, int topRight, int bottomLeft, int bottomRight);

	void processFrustumCulling(Vector3* heightMap, Frustum& f);
	int isInFrustum(Vector3* heightMap, Frustum& f)  const;
	int genrateTriangleIndex(int numTriangles, void* indices, Vector3* heightMap, const Frustum& f, float LODRatio);

	// inline
	QuadTree* createChild(int topLeft, int topRight, int bottomLeft, int bottomRight) const;
	float getDistance(const Vector3& v1, const Vector3& v2) const;
	int getLODLevel(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const;
	bool isVisible(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const;

private:
	QuadTree*	_child[4];
	int			_coner[4];
	int			_center;

	bool		_culled;
	float		_radius;
};

inline QuadTree* QuadTree::createChild(int topLeft, int topRight, int bottomLeft, int bottomRight) const
{
	return new QuadTree(topLeft, topRight, bottomLeft, bottomRight);
}

inline float QuadTree::getDistance(const Vector3& v1, const Vector3& v2) const
{
	return D3DXVec3Length(&(v1 - v2));
}

inline int QuadTree::getLODLevel(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const
{
	float d = getDistance(*(heightMap + _center), cameraPos);
	return max((int)(d * LODRatio), 1);
}

inline bool QuadTree::isVisible(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const
{
	return (_coner[TOP_RIGHT] - _coner[TOP_LEFT] <=  getLODLevel(heightMap, cameraPos, LODRatio));
}