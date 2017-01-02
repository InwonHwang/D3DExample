//https://msdn.microsoft.com/en-us/library/windows/desktop/bb172972(v=vs.85).aspx ����
//https://www.gamedev.net/topic/640029-rotating-about-local-axes/ Local ��ǥ�� ����
#pragma once

#include <d3dx9.h>

#include "../Math/Math.h"

//class Transform
//{
//	Vector3		_position;			/// ī�޶��� ���� ��ġ
//	Vector3		_lookAt;		/// ī�޶��� �ü� ��ġ
//	Vector3		_axisY;			/// ī�޶��� ��溤��
//
//	Vector3		_axisZ;		/// ī�޶� ���ϴ� �������⺤��
//	Vector3		_axisX;		/// ī������ ���麤�� cross( view, up )
//
//	D3DXMATRIX	_transformMatrix;		/// ī�޶� ���
//
//public:
//
//	/// ������
//	Transform();
//
//	/// ī�޶� ����� ����.
//	D3DXMATRIX*	GetViewMatrix() { return &_transformMatrix; }
//
//
//public:
//
//	/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
//	D3DXMATRIX*	SetView(Vector3* pvEye, Vector3* pvLookat, Vector3* pvUp);
//
//	/// ī�޶��� ��ġ���� �����Ѵ�.
//	void			SetEye(Vector3* pv) { _position = *pv; }
//
//	/// ī�޶��� ��ġ���� ����.
//	D3DXVECTOR3*	GetEye() { return &_position; }
//
//	/// ī�޶��� �ü����� �����Ѵ�.
//	void			SetLookat(Vector3* pv) { _lookAt = *pv; }
//
//	/// ī�޶��� �ü����� ����.
//	D3DXVECTOR3*	GetLookat() { return &_lookAt; }
//
//	/// ī�޶��� ��溤�Ͱ��� �����Ѵ�.
//	void			SetUp(Vector3* pv) { _axisY = *pv; }
//
//	/// ī�޶��� ��溤�Ͱ��� ����.
//	D3DXVECTOR3*	GetUp() { return &_axisY; }
//
//	/// ���� �����Ѵ�.
//	void			Flush() { SetView(&_position, &_lookAt, &_axisY); }
//
//	/// ī�޶� ��ǥ���� X������ angle��ŭ ȸ���Ѵ�.
//	D3DXMATRIX*	RotateLocalX(float angle);
//
//	/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
//	D3DXMATRIX*	RotateLocalY(float angle);
//
//	//	D3DXMATRIX*	RotateLocalZ( float angle );
//
//	/// ������ǥ���� *pv���� ��ġ�� �̵��Ѵ�.
//	D3DXMATRIX*	MoveTo(const Vector3& pv);
//
//	/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
//	D3DXMATRIX*	MoveLocalX(float dist);
//
//	/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
//	D3DXMATRIX*	MoveLocalY(float dist);
//
//	/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
//	D3DXMATRIX*	MoveLocalZ(float dist);
//};

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