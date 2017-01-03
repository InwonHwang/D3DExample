#include "Transform.h"
#include "../../Core/Core.h"
#include "../Device.h"

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