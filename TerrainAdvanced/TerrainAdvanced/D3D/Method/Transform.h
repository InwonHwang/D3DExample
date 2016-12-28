//https://msdn.microsoft.com/en-us/library/windows/desktop/bb172972(v=vs.85).aspx 참조
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
	
	virtual void init();
	virtual void update();

	Vector3 getAxisZ();
	Vector3 getScale() const;
	Vector3 getLocalScale() const;	
	Quaternion getRotation() const;
	Quaternion getLocalRotation() const;	
	Vector3 getPosition() const;
	Vector3 getLocalPosition() const;

	// 로컬 transform
	void setLocalScale(float x, float y, float z);
	void setLocalRotation(float x, float y, float z);
	void setLocalPosition(float x, float y, float z);
	void setLocalScale(const Vector3& scale);	
	void setLocalRotation(const Vector3& rotation);
	void setLocalRotation(const Quaternion& rotation);
	void setLocalPosition(const Vector3& position);
	
	// 월드 transform
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setPosition(const Vector3& position);
	void setRotation(const Vector3& rotation);
	void setRotation(const Quaternion& rotation);
	
	void setParent(const Transform& parent);

	D3DXMATRIX matrix() { return _transformData.TransformMatrix; }
	Transform* getChild(int index);
	UINT getChildCount() const;

private:	
	Vector3 internalMatrixToScale(const D3DXMATRIX& matrix) const;
	Quaternion internalMatrixToRotation(const D3DXMATRIX& matrix) const;
	Vector3 internalMatrixToPosition(const D3DXMATRIX& matrix) const;

	void internalSetLocalScale(float x, float y, float z);
	void internalSetLocalRotation(float x, float y, float z);	
	void internalSetLocalRotation(const Quaternion& q);
	void internalSetLocalPosition(float x, float y, float z);
	

	void flush();

private:
	TransformData _transformData;
	UINT		  _childCount;
};

inline UINT Transform::getChildCount() const
{
	return _childCount;
}

inline Vector3 Transform::getAxisZ()
{	
	Quaternion r = getLocalRotation();
	Vector3 angle = Quaternion::ToEulerAngle(r);

	return Quaternion::Rotate(Vector3(0, 0, 1), angle.x, angle.y, angle.z);	
}
