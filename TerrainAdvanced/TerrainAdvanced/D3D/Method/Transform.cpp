#include "Transform.h"
#include "../../Core/Core.h"

Transform::Transform()
{	
}

Transform::~Transform()
{
}

void Transform::init()
{
	memset(&_transformData, 0, sizeof(TransformData));
	D3DXMatrixIdentity(&_transformData.ScaleMatrix);
	D3DXMatrixIdentity(&_transformData.RotationMatrix);
	D3DXMatrixIdentity(&_transformData.PositionMatrix);
	D3DXMatrixIdentity(&_transformData.TransformMatrix);
	D3DXMatrixIdentity(&_transformData.CombTransformMatrix);
}

void Transform::update()
{
	flush();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Get Local SRP ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Transform::getScale() const
{
	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;

	return internalMatrixToScale(m);
}

Vector3 Transform::getLocalScale() const
{
	return internalMatrixToScale(_transformData.TransformMatrix);
}

Quaternion Transform::getRotation() const
{
	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;

	return internalMatrixToRotation(m);
}

Quaternion Transform::getLocalRotation() const
{
	return internalMatrixToRotation(_transformData.TransformMatrix);
}

Vector3 Transform::getPosition() const
{
	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;
	
	return  internalMatrixToPosition(m);
}

Vector3 Transform::getLocalPosition() const
{
	return internalMatrixToPosition(_transformData.TransformMatrix);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set Local SRP ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void Transform::setLocalScale(float x, float y, float z)
{
	internalSetLocalScale(x, y, z);
}
void Transform::setLocalRotation(float x, float y, float z)
{
	internalSetLocalRotation(x * Mathf::DegToRad(), y * Mathf::DegToRad(), z * Mathf::DegToRad());
}
void Transform::setLocalPosition(float x, float y, float z)
{
	internalSetLocalPosition(x, y, z);
}

void Transform::setLocalScale(const Vector3& scale)
{	
	internalSetLocalScale(scale.x, scale.y, scale.z);
}

void Transform::setLocalRotation(const Quaternion& rotation)
{
	internalSetLocalRotation(rotation);
}

void Transform::setLocalRotation(const Vector3& rotation)
{
	internalSetLocalRotation(rotation.x * Mathf::DegToRad(), rotation.y * Mathf::DegToRad(), rotation.z * Mathf::DegToRad());
}

void Transform::setLocalPosition(const Vector3& position)
{
	internalSetLocalPosition(position.x, position.y, position.z);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set SRP /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void Transform::setPosition(float x, float y, float z)
{
	Vector3 t = internalMatrixToPosition(_transformData.CombTransformMatrix);
	Vector3 temp = t + Vector3(x, y, z);

	internalSetLocalPosition(temp.x, temp.y, temp.z);
}

void Transform::setRotation(float x, float y, float z)
{
	Quaternion q = internalMatrixToRotation(_transformData.CombTransformMatrix);
	Vector3 v = Quaternion::ToEulerAngle(q) + Vector3(x,y,z);

	internalSetLocalRotation(v.x * Mathf::DegToRad(), v.y * Mathf::DegToRad(), v.z * Mathf::DegToRad());
}

void Transform::setPosition(const Vector3& position)
{
	Vector3 t = internalMatrixToPosition(_transformData.CombTransformMatrix);
	Vector3 temp = t + position;

	internalSetLocalPosition(temp.x, temp.y, temp.z);
}

void Transform::setRotation(const Vector3& rotation)
{
	Quaternion q = internalMatrixToRotation(_transformData.CombTransformMatrix);
	Vector3 v = Quaternion::ToEulerAngle(q) + rotation;

	internalSetLocalRotation(v.x * Mathf::DegToRad(), v.y * Mathf::DegToRad(), v.z * Mathf::DegToRad());
}

void Transform::setRotation(const Quaternion& rotation)
{	
	Quaternion q = internalMatrixToRotation(_transformData.CombTransformMatrix);
	Vector3 v = Quaternion::ToEulerAngle(q) + Quaternion::ToEulerAngle(rotation);

	internalSetLocalRotation(v.x * Mathf::DegToRad(), v.y * Mathf::DegToRad(), v.z * Mathf::DegToRad());
}

////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////

void Transform::setParent(const Transform& parent)
{
}

Transform* Transform::getChild(int index)
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////
// internal Method /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Transform::internalMatrixToScale(const D3DXMATRIX& matrix) const
{
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return s;
}

Quaternion Transform::internalMatrixToRotation(const D3DXMATRIX& matrix) const
{
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return r;
}

Vector3 Transform::internalMatrixToPosition(const D3DXMATRIX& matrix) const
{
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return t;
}

void Transform::internalSetLocalScale(float x, float y, float z)
{
	D3DXMatrixScaling(&_transformData.ScaleMatrix, x, y, z);	
}

void Transform::internalSetLocalRotation(float x, float y, float z)
{
	D3DXMatrixRotationYawPitchRoll(&_transformData.RotationMatrix, y, x, z);
}

void Transform::internalSetLocalRotation(const Quaternion& q)
{
	D3DXMatrixRotationQuaternion(&_transformData.RotationMatrix, &q);	
}

void Transform::internalSetLocalPosition(float x, float y, float z)
{		
	D3DXMatrixTranslation(&_transformData.PositionMatrix, x, y, z);	
}

void Transform::flush()
{
	_transformData.TransformMatrix = _transformData.ScaleMatrix * _transformData.RotationMatrix * _transformData.PositionMatrix;	
}