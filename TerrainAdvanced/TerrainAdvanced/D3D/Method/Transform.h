#pragma once

#include <d3dx9.h>

#include "../Math/Math.h"

typedef struct TransformData {
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX PositionMatrix;
	D3DXMATRIX TransformMatrix;				// Local Matrix
	D3DXMATRIX CombTransformMatrix;			// Parent Matrix, World Matrix = Parent Matrix * Local Matrix;

	struct TransformData* Parent;
	struct TransformData* Sibling;
	struct TransformData* FirstChild;
}TRANSFORMDATA, * LPTRANSFORMDATA;

class Transform
{
public:
	Transform();
	~Transform();
	
	Vector3 getPosition() const;
	Vector3 getLocalPosition() const;

	// 로컬 transform
	void setLocalScale(const Vector3& scale);	
	void setLocalRotation(const Vector3& rotation);
	void setLocalRotation(const Quaternion& rotation);
	void setLocalPosition(const Vector3& position);
	
	// 월드 transform
	void setPosition(const Vector3& position);
	void setRotation(const Vector3& rotation);
	void setRotation(const Quaternion& rotation);

	Vector3& getAxisX() const;
	Vector3& getAxisY() const;
	Vector3& getAxisZ() const;

	void setParent(const Transform& parent);
	Transform* getChild(int index);
	UINT getChildCount() const;

private:
	void flush();

private:
	TransformData _transformData;
	UINT		  _childCount;
};

inline Vector3& Transform::getAxisX() const
{
	return Vector3(_transformData.TransformMatrix._11, _transformData.TransformMatrix._21, _transformData.TransformMatrix._31);
}

inline Vector3& Transform::getAxisY() const
{
	return Vector3(_transformData.TransformMatrix._12, _transformData.TransformMatrix._22, _transformData.TransformMatrix._32);
}

inline Vector3& Transform::getAxisZ() const
{
	return Vector3(_transformData.TransformMatrix._13, _transformData.TransformMatrix._23, _transformData.TransformMatrix._33);
}

inline UINT Transform::getChildCount() const
{
	return _childCount;
}

