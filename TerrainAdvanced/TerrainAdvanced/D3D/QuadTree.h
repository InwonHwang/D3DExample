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

	enum {
		EDGE_UP,
		EDGE_DN,
		EDGE_LT,
		EDGE_RT
	};

	enum State
	{
		FRUSTUM_OUT,
		FRUSTUM_PARTIALLY_IN,
		FRUSTUM_IN
	};
public:
	QuadTree(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight);
	QuadTree(int cx, int cy);	
	~QuadTree();

	
	bool build(Vector3* heightMap);

	int generateIndex(void* indices, Vector3* heightMap, Frustum& f, float LODRatio);
private:
	// constructor
	void		setConers(int topLeft, int topRight, int bottomLeft, int bottomRight);	

	// destructor
	void		destroy();																

	// build
	bool		buildQuadTree(Vector3* pHeightMap);										
	void		buildNeighbourNode(QuadTree* pRoot, Vector3* pHeightMap, int cx);
	
	// generateIndex
	void		processFrustumCulling(Vector3* heightMap, Frustum& f);
	int			genrateTriangleIndex(int numTriangles, void* indices, Vector3* heightMap, const Frustum& f, float LODRatio);

	// buildQuadTree
	bool		subDivide();

	// buildNeighbourNode
	QuadTree*	findNode(Vector3* pHeightMap, int topLeft, int topRight, int bottomLeft, int bottomRight);
	int			getNodeIndex(int ed, int cx, int& topLeft, int& topRight, int& bottomLeft, int& bottomRight);

	// processFrustumCulling
	int			isInFrustum(Vector3* heightMap, Frustum& f)  const;
	void		setInFrustumAll();

	// inline	
	QuadTree*	createChild(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight) const;
	int			getLODLevel(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const;	
	void		getConer(int& topLeft, int& topRight, int& bottomLeft, int& bottomRight) const;
	bool		isVisible(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const;

	

private:
	QuadTree*	_child[4];
	QuadTree*	_parent;
	QuadTree*	_neighbour[4];

	int			_coner[4];
	int			_center;
	bool		_culled;
	float		_radius;
};

inline QuadTree* QuadTree::createChild(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight) const
{
	return new QuadTree(parent, topLeft, topRight, bottomLeft, bottomRight);
}

inline int QuadTree::getLODLevel(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const
{
	float d = Vector3::GetDistance(*(heightMap + _center), cameraPos);
	return max((int)(d * LODRatio), 1);
}

inline bool QuadTree::isVisible(Vector3* heightMap, const Vector3& cameraPos, float LODRatio) const
{
	return (_coner[TOP_RIGHT] - _coner[TOP_LEFT] <=  getLODLevel(heightMap, cameraPos, LODRatio));
}

inline void QuadTree::getConer(int& topLeft, int& topRight, int& bottomLeft, int& bottomRight) const
{
	topLeft = _coner[0];
	topRight = _coner[1];
	bottomLeft = _coner[2];
	bottomRight = _coner[3];
}