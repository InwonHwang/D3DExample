#pragma once
#include "..\..\Core\Core.h"

class TransformData : public ResourceItem
{
public:
	TransformData(ResourceHandle handle, ResourcePoolImpl* pool);
	~TransformData();

	virtual void Destroy() override;

	void SetMatrix(const D3DXMATRIX& matrix);
	void SetMatrixParent(const D3DXMATRIX& matrix);
	D3DXMATRIX GetMatrix() const;
	D3DXMATRIX GetMatrixParent() const;

private:
	D3DXMATRIX _transform;
	D3DXMATRIX _transformParent;
};

inline void TransformData::SetMatrix(const D3DXMATRIX& matrix)
{
	_transform = matrix;
}

inline void TransformData::SetMatrixParent(const D3DXMATRIX& matrix)
{
	_transformParent = matrix;
}

inline D3DXMATRIX TransformData::GetMatrix() const
{
	return _transform;
}

inline D3DXMATRIX TransformData::GetMatrixParent() const
{
	return _transformParent;
}

