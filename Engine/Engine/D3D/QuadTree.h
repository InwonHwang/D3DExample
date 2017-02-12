#pragma once
#include "..\Core\Core.h"

class Frustum;

class QuadTree
{
	typedef std::vector<Vector3> HeightMapVec;

	enum eConer {
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};

	enum eEdge {
		UP,		// À­º¯
		DN,		// ¾Æ·§º¯
		LT,		// ¿ÞÂÊ
		RT		// ¿À¸¥ÂÊ
	};

	enum eState {
		FRUSTUM_OUT,
		FRUSTUM_PARTIALLY_IN,
		FRUSTUM_IN
	};
public:
	QuadTree(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight);
	QuadTree(int width, int cy);
	~QuadTree();


	bool Build(sp<HeightMapVec> pHeightMapVec);
	int GenerateIndex(void* pIndices, sp<HeightMapVec> pHeightMapVec, sp<Frustum> pFrustum, float LODRatio);

private:
	// constructor
	void		SetConers(int topLeft, int topRight, int bottomLeft, int bottomRight);

	// destructor
	void		Destroy();

	// build
	bool		BuildQuadTree(sp<HeightMapVec> pHeightMapVec);
	void		BuildNeighbourNode(QuadTree* pRoot, sp<HeightMapVec> pHeightMap, int width);

	// generateIndex
	void		ProcessFrustumCulling(sp<HeightMapVec> pHeightMapVec, sp<Frustum> pFrustum);
	int			GenrateTriangleIndex(int numTriangles, void* pIndices, sp<HeightMapVec> pHeightMapVec, const sp<Frustum> pFrustum, float LODRatio);

	// buildQuadTree
	bool		SubDivide();

	// buildNeighbourNode
	QuadTree*	FindNode(sp<HeightMapVec> pHeightMapVec, int topLeft, int topRight, int bottomLeft, int bottomRight);
	bool		GetNeighbourConers(int edgeType, int width, int& topLeft, int& topRight, int& bottomLeft, int& bottomRight);

	// processFrustumCulling
	int			IsInFrustum(sp<HeightMapVec> pHeightMapVec, sp<Frustum> pFrustum)  const;
	void		SetInFrustumAll();

	// inline	
	QuadTree*	CreateChild(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight) const;
	int			GetLODLevel(sp<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const;
	void		GetConers(int& topLeft, int& topRight, int& bottomLeft, int& bottomRight) const;
	bool		IsVisible(sp<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const;

private:
	QuadTree*	_child[4];
	QuadTree*	_parent;
	QuadTree*	_neighbour[4];

	int			_coner[4];
	int			_center;
	bool		_culled;
	float		_radius;
};

inline QuadTree* QuadTree::CreateChild(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight) const
{
	return new QuadTree(parent, topLeft, topRight, bottomLeft, bottomRight);
}

inline int QuadTree::GetLODLevel(const sp<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	float d = Vector3::GetDistance(pHeightMapVec->data()[_center], cameraPos);
	return max((int)(d * LODRatio), 1);
}

inline bool QuadTree::IsVisible(const sp<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	return (_coner[TOP_RIGHT] - _coner[TOP_LEFT] <= GetLODLevel(pHeightMapVec, cameraPos, LODRatio));
}

inline void QuadTree::GetConers(int& topLeft, int& topRight, int& bottomLeft, int& bottomRight) const
{
	topLeft = _coner[0];
	topRight = _coner[1];
	bottomLeft = _coner[2];
	bottomRight = _coner[3];
}

