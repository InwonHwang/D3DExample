#pragma once
#include "..\..\Core\Core.h"

class TransformData : public ResourceItem
{
public:
	TransformData(ResourceHandle handle, ResourcePoolImpl* pool);
	~TransformData();

	virtual void Destroy() override;

	// friend class SpriteRenderer private함수로 바꾸기
	void SetMatrixLocal(const D3DXMATRIX& matrix);
	void SetMatrixWorldParent(const D3DXMATRIX& matrix);
	D3DXMATRIX GetMatrixLocal() const;
	D3DXMATRIX GetMatrixWorldParent() const;

private:	
	D3DXMATRIX _matLocal;
	D3DXMATRIX _matWorldParent;
};

inline void TransformData::SetMatrixLocal(const D3DXMATRIX& matrix)
{
	_matLocal = matrix;
}

inline void TransformData::SetMatrixWorldParent(const D3DXMATRIX& matrix)
{
	_matWorldParent = matrix;
}


inline D3DXMATRIX TransformData::GetMatrixLocal() const
{
	return _matLocal;
}

inline D3DXMATRIX TransformData::GetMatrixWorldParent() const
{
	return _matWorldParent;
}

