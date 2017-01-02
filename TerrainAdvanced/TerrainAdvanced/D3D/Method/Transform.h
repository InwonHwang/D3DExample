//https://msdn.microsoft.com/en-us/library/windows/desktop/bb172972(v=vs.85).aspx 참조
//https://www.gamedev.net/topic/640029-rotating-about-local-axes/ Local 좌표계 참조
#pragma once

#include <d3dx9.h>

#include "../Math/Math.h"

//class Transform
//{
//	Vector3		_position;			/// 카메라의 현재 위치
//	Vector3		_lookAt;		/// 카메라의 시선 위치
//	Vector3		_axisY;			/// 카메라의 상방벡터
//
//	Vector3		_axisZ;		/// 카메라가 향하는 단위방향벡터
//	Vector3		_axisX;		/// 카마레의 측면벡터 cross( view, up )
//
//	D3DXMATRIX	_transformMatrix;		/// 카메라 행렬
//
//public:
//
//	/// 생성자
//	Transform();
//
//	/// 카메라 행렬을 얻어낸다.
//	D3DXMATRIX*	GetViewMatrix() { return &_transformMatrix; }
//
//
//public:
//
//	/// 카메라 행렬을 생성하기위한 기본 벡터값들을 설정한다.
//	D3DXMATRIX*	SetView(Vector3* pvEye, Vector3* pvLookat, Vector3* pvUp);
//
//	/// 카메라의 위치값을 설정한다.
//	void			SetEye(Vector3* pv) { _position = *pv; }
//
//	/// 카메라의 위치값을 얻어낸다.
//	D3DXVECTOR3*	GetEye() { return &_position; }
//
//	/// 카메라의 시선값을 설정한다.
//	void			SetLookat(Vector3* pv) { _lookAt = *pv; }
//
//	/// 카메라의 시선값을 얻어낸다.
//	D3DXVECTOR3*	GetLookat() { return &_lookAt; }
//
//	/// 카메라의 상방벡터값을 설정한다.
//	void			SetUp(Vector3* pv) { _axisY = *pv; }
//
//	/// 카메라의 상방벡터값을 얻어낸다.
//	D3DXVECTOR3*	GetUp() { return &_axisY; }
//
//	/// 값을 갱신한다.
//	void			Flush() { SetView(&_position, &_lookAt, &_axisY); }
//
//	/// 카메라 좌표계의 X축으로 angle만큼 회전한다.
//	D3DXMATRIX*	RotateLocalX(float angle);
//
//	/// 카메라 좌표계의 Y축으로 angle만큼 회전한다.
//	D3DXMATRIX*	RotateLocalY(float angle);
//
//	//	D3DXMATRIX*	RotateLocalZ( float angle );
//
//	/// 월드좌표계의 *pv값의 위치로 이동한다.
//	D3DXMATRIX*	MoveTo(const Vector3& pv);
//
//	/// 카메라 좌표계의 X축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
//	D3DXMATRIX*	MoveLocalX(float dist);
//
//	/// 카메라 좌표계의 Y축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
//	D3DXMATRIX*	MoveLocalY(float dist);
//
//	/// 카메라 좌표계의 Z축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
//	D3DXMATRIX*	MoveLocalZ(float dist);
//};

// 저장용
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

	virtual void create();							// 내부 데이터 생성
	virtual void destroy();							// 내부 데이터 해제

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