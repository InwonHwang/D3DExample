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
		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		Vector3 v = pHeightMapVec->data()[_coner[0]] - pHeightMapVec->data()[_coner[3]];
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
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
		// �̿������ 4�� �ڳʰ��� ��´�.
		isNeighbourVaild = GetNeighbourConers(i, width, topLeft, topRight, bottomLeft, bottomRight);
		// �ڳʰ����� �̿������ �����͸� ���´�.

		if (isNeighbourVaild)
		{
			_neighbour[i] = pRoot->FindNode(pHeightMapVec, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	// �ڽĳ��� ���ȣ��
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
	case FRUSTUM_OUT:				// �������ҿ��� �������, ������� �˻� �ʿ����
		_culled = true;
		return;
	case FRUSTUM_IN:	// �������ҿ� ��������, ������� �˻� �ʿ����
		SetInFrustumAll();
		return;
	case FRUSTUM_PARTIALLY_IN:		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
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

		if (!b[UP]) // ��� �κк����� �ʿ��Ѱ�?
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
		else	// ��� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _coner[2];
			index._2 = _coner[3];
			*p++ = index;
			numTriangles++;
		}

		if (!b[DN]) // �ϴ� �κк����� �ʿ��Ѱ�?
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
		else	// �ϴ� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _coner[1];
			index._2 = _coner[0];
			*p++ = index;
			numTriangles++;
		}

		if (!b[LT]) // ���� �κк����� �ʿ��Ѱ�?
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
		else	// ���� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _coner[0];
			index._2 = _coner[2];			
			*p++ = index;
			numTriangles++;
		}

		if (!b[RT]) // ���� �κк����� �ʿ��Ѱ�?
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
		else	// ���� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _coner[3];
			index._2 = _coner[1];			
			*p++ = index;
			numTriangles++;
		}

		return numTriangles;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
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

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pHeightMapVec->data()[n].x;
		pt.y = (int)pHeightMapVec->data()[n].z;

		for (int i = 0; i < 4; i++)
		{
			SetRect(&rc, (int)pHeightMapVec->data()[_child[i]->_coner[2]].x,
						(int)pHeightMapVec->data()[_child[i]->_coner[2]].z,
						(int)pHeightMapVec->data()[_child[i]->_coner[1]].x,
						(int)pHeightMapVec->data()[_child[i]->_coner[1]].z);
			// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
			if ((rc.left <= pt.x) && (pt.x <= rc.right) && (rc.bottom <= pt.y) && (pt.y <= rc.top))
				return _child[i]->FindNode(pHeightMapVec, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	return nullptr;
}

bool QuadTree::GetNeighbourConers(int edgeType, int width, int& bottomLeft, int& bottomRight, int& topLeft, int& topRight)
{
	int edgeSize = _coner[1] - _coner[0];	// ���� ����� �¿�����

	switch (edgeType)
	{
	case UP:	// ���� ���� �̿������ �ε���
		topLeft = _coner[2] + width * edgeSize;
		topRight = _coner[3] + width * edgeSize;
		bottomLeft = _coner[2];
		bottomRight = _coner[3];
		break;
	case DN:	// �Ʒ� ���� �̿������ �ε���
		topLeft = _coner[0];
		topRight = _coner[1];
		bottomLeft = _coner[0] - width * edgeSize;
		bottomRight = _coner[1] - width * edgeSize;
		break;
	case LT:	// ���� ���� �̿������ �ε���
		topLeft = _coner[2] - edgeSize;
		topRight = _coner[2];
		bottomLeft = _coner[0] - edgeSize;
		bottomRight = _coner[0];
		break;
	case RT:	// ���� ���� �̿������ �ε���
		topLeft = _coner[3];
		topRight = _coner[3] + edgeSize;
		bottomLeft = _coner[1];
		bottomRight = _coner[1] + edgeSize;
		break;
	}

	bool b[4] = { false, };
	b[0] = topRight > width * width;		// ���� �̿���尡 HeightMap ������ ���.
	b[1] = bottomLeft < 0;					// �Ʒ��� �̿���尡 HeightMap ������ ���.
	b[2] = topLeft % width == width - 1;	// ���� �̿���尡 HeightMap ������ ���.
	b[3] = topRight % width == 0;			// ������ �̿���尡 HeightMap ������ ���.

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




