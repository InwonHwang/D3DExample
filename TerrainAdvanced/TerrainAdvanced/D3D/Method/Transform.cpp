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
	D3DXMatrixIdentity(&_data->ScaleMatrix);
	D3DXMatrixIdentity(&_data->RotationMatrix);
	D3DXMatrixIdentity(&_data->TranslationMatrix);	
	D3DXMatrixIdentity(&_data->TransformMatrix);
	D3DXMatrixIdentity(&_data->CombTransformMatrix);
}

void Transform::destroy()
{
	_data->Parent = nullptr;
	_data->Sibling = nullptr;
	_data->FirstChild = nullptr;
	SAFE_DELETE(_data);
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

void Transform::setLocalRotation(const Vector3& rotation)
{
	internalSetRotation(rotation);
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
	return internalMatrixToScale(_data->ScaleMatrix);
}

Quaternion Transform::getLocalRotation() const
{
	return internalMatrixToRotation(_data->RotationMatrix);
}

Vector3 Transform::getLocalEulerAngle() const
{
	return Quaternion::ToEulerAngle(internalMatrixToRotation(_data->RotationMatrix));
}

Vector3 Transform::getLocalPosition() const
{
	return internalMatrixToTranslation(_data->TranslationMatrix);
}

Vector3 Transform::getScale() const
{
	D3DXMATRIX temp;
	D3DXMatrixMultiply(&temp, &_data->ScaleMatrix, &_data->CombTransformMatrix);

	return internalMatrixToScale(temp);
}

Quaternion Transform::getRotation() const
{
	D3DXMATRIX temp;
	D3DXMatrixMultiply(&temp, &_data->RotationMatrix, &_data->CombTransformMatrix);

	return internalMatrixToRotation(temp);
}

Vector3 Transform::getEulerAngle() const
{
	D3DXMATRIX temp;
	D3DXMatrixMultiply(&temp, &_data->RotationMatrix, &_data->CombTransformMatrix);

	return Quaternion::ToEulerAngle(internalMatrixToRotation(temp));
}

Vector3 Transform::getPosition() const
{
	D3DXMATRIX temp;
	D3DXMatrixMultiply(&temp, &_data->TranslationMatrix, &_data->CombTransformMatrix);

	return internalMatrixToTranslation(temp);
}


//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Private SRT Method////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void Transform::internalSetScale(const Vector3& s)
{
	D3DXMatrixScaling(&_data->ScaleMatrix, s.x, s.y, s.z);
}

void Transform::internalSetRotation(const Vector3& r)
{
	D3DXMatrixRotationYawPitchRoll(&_data->RotationMatrix, r.y * Mathf::DegToRad(), r.x * Mathf::DegToRad(), r.z * Mathf::DegToRad());
}

void Transform::internalSetRotation(const Quaternion& q)
{
	D3DXMatrixRotationQuaternion(&_data->RotationMatrix, &q);
}

void Transform::internalSetTranslation(const Vector3& t)
{
	D3DXMatrixTranslation(&_data->TranslationMatrix, t.x, t.y, t.z);
}

void Transform::internalUpdateMatrix()
{
	D3DXMatrixIdentity(&_data->TransformMatrix);
	D3DXMatrixMultiply(&_data->TransformMatrix, &_data->ScaleMatrix, &_data->TransformMatrix);	
	D3DXMatrixMultiply(&_data->TransformMatrix, &_data->RotationMatrix, &_data->TransformMatrix);
	D3DXMatrixMultiply(&_data->TransformMatrix, &_data->TranslationMatrix, &_data->TransformMatrix);
	
	
}

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

Vector3 Transform::internalMatrixToTranslation(const D3DXMATRIX& matrix) const
{
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return t;
}


//void Transform::init()
//{
//	memset(&_transformData, 0, sizeof(TransformData));
//	D3DXMatrixIdentity(&_transformData.ScaleMatrix);
//	D3DXMatrixIdentity(&_transformData.RotationMatrix);
//	D3DXMatrixIdentity(&_transformData.PositionMatrix);
//	D3DXMatrixIdentity(&_transformData.TransformMatrix);
//	D3DXMatrixIdentity(&_transformData.CombTransformMatrix);
//}
//
//void Transform::update()
//{
//	flush();
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////
//// Get Local SRP ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//
//Vector3 Transform::getScale() const
//{
//	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;
//
//	return internalMatrixToScale(m);
//}
//
//Vector3 Transform::getLocalScale() const
//{
//	return internalMatrixToScale(_transformData.TransformMatrix);
//}
//
//Quaternion Transform::getRotation() const
//{
//	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;
//
//	return internalMatrixToRotation(m);
//}
//
//Quaternion Transform::getLocalRotation() const
//{
//	return internalMatrixToRotation(_transformData.RotationMatrix);
//}
//
//Vector3 Transform::getEulerAngle() const
//{
//	return Quaternion::ToEulerAngle(getRotation());
//}
//Vector3 Transform::getLocalEulerAngle() const
//{
//	return Quaternion::ToEulerAngle(getLocalRotation());
//}
//
//Vector3 Transform::getPosition() const
//{
//	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;
//	
//	return  internalMatrixToPosition(m);
//}
//
//Vector3 Transform::getLocalPosition() const
//{
//	return internalMatrixToPosition(_transformData.TransformMatrix);
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////
//// Set Local SRP ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//
//void Transform::setLocalScale(float x, float y, float z)
//{
//	internalSetLocalScale(x, y, z);
//}
//void Transform::setLocalRotation(float x, float y, float z)
//{
//	internalSetLocalRotation(x, y, z);
//}
//void Transform::setLocalPosition(float x, float y, float z)
//{
//	internalSetLocalPosition(x, y, z);
//}
//
//void Transform::setLocalScale(const Vector3& scale)
//{	
//	internalSetLocalScale(scale.x, scale.y, scale.z);
//}
//
//void Transform::setLocalRotation(const Quaternion& rotation)
//{
//	internalSetLocalRotation(rotation);
//}
//
//void Transform::setLocalRotation(const Vector3& rotation)
//{
//	internalSetLocalRotation(rotation.x, rotation.y, rotation.z);
//}
//
//void Transform::setLocalPosition(const Vector3& position)
//{
//	internalSetLocalPosition(position.x, position.y, position.z);
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////
//// Set SRP /////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//
//void Transform::setPosition(float x, float y, float z)
//{
//	Vector3 t = internalMatrixToPosition(_transformData.CombTransformMatrix);
//	Vector3 temp = t + Vector3(x, y, z);	
//
//	internalSetLocalPosition(temp.x, temp.y, temp.z);
//}
//
//void Transform::setRotation(float x, float y, float z)
//{
//	Quaternion q = internalMatrixToRotation(_transformData.CombTransformMatrix);
//	Vector3 v = Quaternion::ToEulerAngle(q) + Vector3(x,y,z);
//
//	internalSetLocalRotation(v.x, v.y, v.z);
//}
//
//void Transform::setPosition(const Vector3& position)
//{
//	Vector3 t = internalMatrixToPosition(_transformData.CombTransformMatrix);
//	Vector3 temp = t + position;
//
//	internalSetLocalPosition(temp.x, temp.y, temp.z);
//}
//
//void Transform::setRotation(const Vector3& rotation)
//{
//	Quaternion q = internalMatrixToRotation(_transformData.CombTransformMatrix);
//	Vector3 v = Quaternion::ToEulerAngle(q) + rotation;
//
//	internalSetLocalRotation(v.x, v.y, v.z);
//}
//
//void Transform::setRotation(const Quaternion& rotation)
//{	
//	Quaternion q = internalMatrixToRotation(_transformData.CombTransformMatrix);
//	Vector3 v = Quaternion::ToEulerAngle(q) + Quaternion::ToEulerAngle(rotation);
//
//	internalSetLocalRotation(v.x, v.y, v.z);
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////
//// 
//////////////////////////////////////////////////////////////////////////////////////////////
//
//void Transform::setParent(const Transform& parent)
//{
//}
//
//Transform* Transform::getChild(int index)
//{
//	return nullptr;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////
//// internal Method /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//
//Vector3 Transform::internalMatrixToScale(const D3DXMATRIX& matrix) const
//{
//	Vector3 s, t;
//	Quaternion r;
//	D3DXMatrixDecompose(&s, &r, &t, &matrix);
//
//	return s;
//}
//
//Quaternion Transform::internalMatrixToRotation(const D3DXMATRIX& matrix) const
//{
//	Vector3 s, t;
//	Quaternion r;
//	D3DXMatrixDecompose(&s, &r, &t, &matrix);
//
//	return r;
//}
//
//Vector3 Transform::internalMatrixToPosition(const D3DXMATRIX& matrix) const
//{
//	Vector3 s, t;
//	Quaternion r;
//	D3DXMatrixDecompose(&s, &r, &t, &matrix);
//
//	return t;
//}
//
//void Transform::internalSetLocalScale(float x, float y, float z)
//{
//	D3DXMatrixScaling(&_transformData.ScaleMatrix, x, y, z);
//}
//
//void Transform::internalSetLocalRotation(float x, float y, float z)
//{
//	D3DXMatrixRotationYawPitchRoll(&_transformData.RotationMatrix, y, x, z);
//	//D3DXMatrixMultiply();
//}
//
//void Transform::internalSetLocalRotation(const Quaternion& q)
//{	
//	D3DXMatrixRotationQuaternion(&_transformData.RotationMatrix, &q);
//}
//
//void Transform::internalSetLocalPosition(float x, float y, float z)
//{		
//	D3DXMatrixTranslation(&_transformData.PositionMatrix, x, y, z);
//}
//
//void Transform::flush()
//{	
//	D3DXMatrixMultiply(&_transformData.TransformMatrix, &_transformData.TransformMatrix, &_transformData.ScaleMatrix);
//	D3DXMatrixMultiply(&_transformData.TransformMatrix, &_transformData.TransformMatrix, &_transformData.RotationMatrix);	
//	D3DXMatrixMultiply(&_transformData.TransformMatrix, &_transformData.TransformMatrix, &_transformData.PositionMatrix);
//	//_transformData.TransformMatrix = _transformData.ScaleMatrix * _transformData.RotationMatrix * _transformData.PositionMatrix;	
//}