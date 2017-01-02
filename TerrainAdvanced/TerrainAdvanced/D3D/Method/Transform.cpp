#include "Transform.h"
#include "../../Core/Core.h"
#include "../Device.h"

//Transform::Transform()
//{
//	Vector3	eye(0.0f, 0.0f, 0.0f);
//	Vector3	lookat(0.0f, 0.0f, -1.0f);
//	Vector3	up(0.0f, 1.0f, 0.0f);
//	D3DXMatrixIdentity(&_transformMatrix);
//	SetView(&eye, &lookat, &up);
//}
//
///// 카메라 행렬을 생성하기위한 기본 벡터값들을 설정한다.
//D3DXMATRIX*	Transform::SetView(Vector3* pvEye, Vector3* pvLookat, Vector3* pvUp)
//{
//	_position = *pvEye;
//	_lookAt = *pvLookat;
//	_axisY = *pvUp;
//	D3DXVec3Normalize(&_axisZ, &(_lookAt - _position));
//	D3DXVec3Cross(&_axisX, &_axisY, &_axisZ);
//
//	D3DXMatrixLookAtLH(&_transformMatrix, &_position, &_lookAt, &_axisY);
//
//	return &_transformMatrix;
//}
//
///// 카메라 좌표계의 X축으로 angle만큼 회전한다.
//D3DXMATRIX* Transform::RotateLocalX(float angle)
//{
//	D3DXMATRIX matRot;
//	D3DXMatrixRotationAxis(&matRot, &_axisX, angle);
//
//	Vector3 vNewDst, vNewUp;
//	D3DXVec3TransformCoord(&vNewDst, &_axisZ, &matRot);	// view * rot로 새로운 dst vector를 구한다.
//															//	D3DXVec3Cross( &vNewUp, &vNewDst, &_axisX );			// cross( dst, x축)으로 up vector를 구한다.
//															//	D3DXVec3Normalize( &vNewUp, &vNewUp );					// up vector를 unit vector로...
//	vNewDst += _position;										// 실제 dst position =  eye Position + dst vector
//
//	return SetView(&_position, &vNewDst, &_axisY);
//}
//
///// 카메라 좌표계의 Y축으로 angle만큼 회전한다.
//D3DXMATRIX* Transform::RotateLocalY(float angle)
//{
//	D3DXMATRIX matRot;
//	D3DXMatrixRotationAxis(&matRot, &_axisY, angle);
//
//	Vector3 vNewDst;
//	D3DXVec3TransformCoord(&vNewDst, &_axisZ, &matRot);	// view * rot로 새로운 dst vector를 구한다.
//	vNewDst += _position;										// 실제 dst position =  eye Position + dst vector
//
//	return SetView(&_position, &vNewDst, &_axisY);
//}
//
///// 카메라 좌표계의 X축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
//D3DXMATRIX* Transform::MoveLocalX(float dist)
//{
//	Vector3 vNewEye = _position;
//	Vector3 vNewDst = _lookAt;
//
//	Vector3 vMove;
//	D3DXVec3Normalize(&vMove, &_axisX);
//	vMove *= dist;
//	vNewEye += vMove;
//	vNewDst += vMove;
//
//	return SetView(&vNewEye, &vNewDst, &_axisY);
//}
//
///// 카메라 좌표계의 Y축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
//D3DXMATRIX* Transform::MoveLocalY(float dist)
//{
//	Vector3 vNewEye = _position;
//	Vector3 vNewDst = _lookAt;
//
//	Vector3 vMove;
//	D3DXVec3Normalize(&vMove, &_axisY);
//	vMove *= dist;
//	vNewEye += vMove;
//	vNewDst += vMove;
//
//	return SetView(&vNewEye, &vNewDst, &_axisY);
//}
//
///// 카메라 좌표계의 Z축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
//D3DXMATRIX* Transform::MoveLocalZ(float dist)
//{
//	Vector3 vNewEye = _position;
//	Vector3 vNewDst = _lookAt;
//
//	Vector3 vMove;
//	D3DXVec3Normalize(&vMove, &_axisZ);
//	vMove *= dist;
//	vNewEye += vMove;
//	vNewDst += vMove;
//
//	return SetView(&vNewEye, &vNewDst, &_axisY);
//}
//
///// 월드좌표계의 *pv값의 위치로 카메라를 이동한다.
//D3DXMATRIX* Transform::MoveTo(const Vector3& pv)
//{
//	Vector3	dv = pv - _position;
//	_position = pv;
//	_lookAt += dv;
//	return SetView(&_position, &_lookAt, &_axisY);
//}


Transform::Transform()
{	
}

Transform::~Transform()
{
}

void Transform::create()
{
	_data = new TransformData();
	_scale = new Vector3(1.0f, 1.0f, 1.0f);
	_rotation = new Quaternion(Quaternion::Euler(0.0f, 0.0f, 0.0f));
	_position = new Vector3(0.0f, 0.0f, 0.0f);
	_axisX = new Vector3(1.0f, 0.0f, 0.0f);
	_axisY = new Vector3(0.0f, 1.0f, 0.0f);
	_axisZ = new Vector3(0.0f, 0.0f, 1.0f);
	_lookAt = new Vector3(0.0f, 0.0f, -1.0f);
}

void Transform::destroy()
{
	SAFE_DELETE(_data);
	SAFE_DELETE(_scale);
	SAFE_DELETE(_rotation);
	SAFE_DELETE(_position);
	SAFE_DELETE(_axisX);
	SAFE_DELETE(_axisY);
	SAFE_DELETE(_axisZ);
	SAFE_DELETE(_lookAt);
}

void Transform::update()
{
	internalUpdateMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Set Local SRT /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void Transform::setLocalScale(const Vector3& scale)
{
	internalSetScale(scale);
}

void  Transform::setLocalRotation(const Quaternion& q)
{
	internalSetRotation(q);
}

void Transform::setLocalPosition(const Vector3& position)
{
	internalSetTranslation(position);
}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Get SRT, Matrix////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

D3DXMATRIX Transform::getMatrix() const
{
	D3DXMATRIX ret;	
	D3DXMatrixMultiply(&ret, &_data->TransformMatrix, &_data->CombTransformMatrix);
	return ret;
}

Vector3 Transform::getLocalScale() const
{
	return internalMatrixToScale(_data->TransformMatrix);
}

Quaternion Transform::getLocalRotation() const
{
	return *_rotation;
}

Vector3 Transform::getLocalEulerAngle() const
{
	return Quaternion::ToEulerAngle(*_rotation);
}

Vector3 Transform::getLocalPosition() const
{
	return *_position;
}

Vector3 Transform::getScale() const
{
	
	return internalMatrixToScale(_data->TransformMatrix);
}

Quaternion Transform::getRotation() const
{
	
	return internalMatrixToRotation(_data->TransformMatrix);
}

Vector3 Transform::getEulerAngle() const
{
	
	return Quaternion::ToEulerAngle(internalMatrixToRotation(_data->TransformMatrix));
}

Vector3 Transform::getPosition() const
{
	
	return internalMatrixToTranslation(_data->TransformMatrix);
}


//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Private SRT Method////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void Transform::internalSetScale(const Vector3& s)
{
	*_scale = s;
}

void Transform::internalSetRotation(const Quaternion& q)
{
	*_rotation = q;

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &q);
	
	D3DXVec3TransformCoord(_axisX, &Vector3(1.0f, 0.0f, 0.0f), &matRot);
	D3DXVec3TransformCoord(_axisY, &Vector3(0.0f, 1.0f, 0.0f), &matRot);
	D3DXVec3TransformCoord(_axisZ, &Vector3(0.0f, 0.0f, 1.0f), &matRot);
	D3DXVec3Normalize(_axisX, _axisX);
	D3DXVec3Normalize(_axisY, _axisY);
	D3DXVec3Normalize(_axisZ, _axisZ);

	*_lookAt = *_position - *_axisZ;
}

void Transform::internalSetTranslation(const Vector3& t)
{
	*_lookAt -= *_position;
	*_position = t;
	*_lookAt += t;
}

void Transform::internalUpdateMatrix()
{
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, _scale->x, _scale->y, _scale->z);
	D3DXMatrixLookAtLH(&_data->TransformMatrix, _position, _lookAt, _axisY);
	D3DXMatrixMultiply(&_data->TransformMatrix, &scale, &_data->TransformMatrix);
}

Vector3 Transform::internalMatrixToScale(const D3DXMATRIX& matrix) const
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return s;
}

Quaternion Transform::internalMatrixToRotation(const D3DXMATRIX& matrix) const
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return r;
}

Vector3 Transform::internalMatrixToTranslation(const D3DXMATRIX& matrix) const
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return t;
}