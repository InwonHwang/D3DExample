//https://msdn.microsoft.com/en-us/library/windows/desktop/bb172972(v=vs.85).aspx ����
//https://www.gamedev.net/topic/640029-rotating-about-local-axes/ Local ��ǥ�� ����
#pragma once

#include <d3dx9.h>

#include "../Math/Math.h"
#include "../Description/Description.h"

// �����
typedef struct TransformData {
	D3DXMATRIX TransformMatrix;				// Local Matrix
	D3DXMATRIX CombTransformMatrix;			// Parent Matrix, World Matrix = Parent Matrix * Local Matrix;

	struct TransformData* Parent;
	struct TransformData* Sibling;
	struct TransformData* FirstChild;
		
	TransformData() : Parent(nullptr), Sibling(nullptr), FirstChild(nullptr)
	{
		D3DXMatrixIdentity(&TransformMatrix);
		D3DXMatrixIdentity(&CombTransformMatrix);
	}
	~TransformData() 
	{
		Parent = nullptr;
		Sibling = nullptr;
		FirstChild = nullptr;
	}
}TRANSFORMDATA, *LPTRANSFORMDATA;

class Transform
{
public:
	Transform();
	~Transform();

	virtual void create();							// ���� ������ ����
	virtual void destroy();							// ���� ������ ����

	virtual void update();

	void setLocalScale(const Vector3& scale);
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
	void internalSetRotation(const Quaternion& q);
	void internalSetTranslation(const Vector3& t);
	void internalUpdateMatrix();

	Vector3 internalMatrixToScale(const D3DXMATRIX& matrix) const;
	Quaternion internalMatrixToRotation(const D3DXMATRIX& matrix) const;
	Vector3 internalMatrixToTranslation(const D3DXMATRIX& matrix) const;

private:
	TransformData*	_data;

	Vector3*		_scale;
	Quaternion*		_rotation;
	Vector3*		_position;

	Vector3*		_axisX;
	Vector3*		_axisY;
	Vector3*		_axisZ;

	Vector3*		_lookAt;
};