//https://msdn.microsoft.com/en-us/library/windows/desktop/bb172972(v=vs.85).aspx 참조
//https://www.gamedev.net/topic/640029-rotating-about-local-axes/ Local 좌표계 참조
#pragma once

#include <d3dx9.h>

#include "../Math/Math.h"

typedef struct TransformData {	
	D3DXMATRIX RotationMatrix;			
	D3DXMATRIX ScaleMatrix;		
	D3DXMATRIX TranslationMatrix;

	D3DXMATRIX TransformMatrix;				// Local Matrix
	D3DXMATRIX CombTransformMatrix;			// Parent Matrix, World Matrix = Parent Matrix * Local Matrix;

	struct TransformData* Parent;
	struct TransformData* Sibling;
	struct TransformData* FirstChild;
}TRANSFORMDATA, *LPTRANSFORMDATA;

class Transform
{
public:
	Transform();
	~Transform();

	virtual void create();							// 내부 데이터 생성
	virtual void destroy();							// 내부 데이터 해제

	virtual void update();

	void setLocalScale(const Vector3& scale);	
	void setLocalRotation(const Vector3& rotation);
	void setLocalRotation(const Quaternion& rotation);
	void setLocalPosition(const Vector3& position);
	
	Vector3 getLocalScale() const;
	Quaternion getLocalRotation() const;
	Vector3 getLocalEulerAngle() const;
	Vector3 getLocalPosition() const;	

	Vector3 getScale() const;
	Quaternion getRotation() const;
	Vector3 getEulerAngle() const;
	Vector3 getPosition() const;

	D3DXMATRIX getMatrix() const;
	

private:
	void internalSetScale(const Vector3& s);
	void internalSetRotation(const Vector3& r);
	void internalSetRotation(const Quaternion& q);
	void internalSetTranslation(const Vector3& t);
	void internalUpdateMatrix();

	Vector3 internalMatrixToScale(const D3DXMATRIX& matrix) const;
	Quaternion internalMatrixToRotation(const D3DXMATRIX& matrix) const;
	Vector3 internalMatrixToTranslation(const D3DXMATRIX& matrix) const;
	
private:
	TransformData*	_data;
	UINT			_childCount;

};




//typedef struct TransformData {
//	D3DXMATRIX ScaleMatrix;
//	D3DXMATRIX RotationMatrix;
//	D3DXMATRIX PositionMatrix;
//	D3DXMATRIX TransformMatrix;				// Local Matrix
//	D3DXMATRIX CombTransformMatrix;			// Parent Matrix, World Matrix = Parent Matrix * Local Matrix;
//
//	struct TransformData* Parent;
//	struct TransformData* Sibling;
//	struct TransformData* FirstChild;
//}TRANSFORMDATA, * LPTRANSFORMDATA;
//
//class Transform
//{
//public:
//	Transform();
//	~Transform();
//	
//	virtual void init();
//	virtual void update();
//
//	Vector3 getScale() const;
//	Vector3 getLocalScale() const;	
//	Quaternion getRotation() const;
//	Quaternion getLocalRotation() const;	
//	Vector3 getEulerAngle() const;
//	Vector3 getLocalEulerAngle() const;
//	Vector3 getPosition() const;
//	Vector3 getLocalPosition() const;
//
//	// 로컬 transform
//	void setLocalScale(float x, float y, float z);
//	void setLocalRotation(float x, float y, float z);
//	void setLocalPosition(float x, float y, float z);
//	void setLocalScale(const Vector3& scale);	
//	void setLocalRotation(const Vector3& rotation);
//	void setLocalRotation(const Quaternion& rotation);
//	void setLocalPosition(const Vector3& position);
//	
//	// 월드 transform
//	void setPosition(float x, float y, float z);
//	void setRotation(float x, float y, float z);
//	void setPosition(const Vector3& position);
//	void setRotation(const Vector3& rotation);
//	void setRotation(const Quaternion& rotation);
//	
//	void setParent(const Transform& parent);
//
//	D3DXMATRIX getMatrix() { return _transformData.TransformMatrix; }
//	Transform* getChild(int index);
//	UINT getChildCount() const;
//
//private:	
//	Vector3 internalMatrixToScale(const D3DXMATRIX& matrix) const;
//	Quaternion internalMatrixToRotation(const D3DXMATRIX& matrix) const;
//	Vector3 internalMatrixToPosition(const D3DXMATRIX& matrix) const;
//
//	void internalSetLocalScale(float x, float y, float z);
//	void internalSetLocalRotation(float x, float y, float z);	
//	void internalSetLocalRotation(const Quaternion& q);
//	void internalSetLocalPosition(float x, float y, float z);
//	
//
//	void flush();
//
//private:
//	TransformData _transformData;
//	UINT		  _childCount;	
//	bool		  _scale;
//	bool		  _rotation;
//	bool		  _translation;
//};
//
//inline UINT Transform::getChildCount() const
//{
//	return _childCount;
//}

