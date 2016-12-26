#include "Transform.h"
#include <tchar.h>
#include <iostream>

//namespace std { typedef basic_string<TCHAR> tstring; }
//
//void DebugBox(HRESULT hr, LPCTSTR str)
//{
//	TCHAR szBuffer[50];
//	_stprintf_s(szBuffer, _T("%i"), hr);
//
//	MessageBox(GetActiveWindow(), szBuffer, str, MB_OK);
//}
//
//std::tstring floatToString(float f)
//{
//	TCHAR szBuffer[50];
//	_stprintf_s(szBuffer, _T("%f"), f);
//
//	return std::tstring(szBuffer);
//}


Transform::Transform()
{
	memset(&_transformData, 0, sizeof(TransformData));
	D3DXMatrixIdentity(&_transformData.ScaleMatrix);
	D3DXMatrixIdentity(&_transformData.RotationMatrix);
	D3DXMatrixIdentity(&_transformData.PositionMatrix);
	D3DXMatrixIdentity(&_transformData.TransformMatrix);
	D3DXMatrixIdentity(&_transformData.CombTransformMatrix);
}

Transform::~Transform()
{
}


Vector3 Transform::getPosition() const
{
	Vector3 s, t;
	Quaternion r;
	D3DXMATRIX m = _transformData.CombTransformMatrix * _transformData.TransformMatrix;
	D3DXMatrixDecompose(&s, &r, &t, &m);

	//DebugBox(0, floatToString(t.x).c_str());
	
	return  t;
}

Vector3 Transform::getLocalPosition() const
{
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &_transformData.PositionMatrix);

	//DebugBox(0, floatToString(t.x).c_str());

	return t;
}

////////////////////////////////////////////////////////////////////////////////////////////

void Transform::setLocalScale(const Vector3& scale)
{	
	D3DXMatrixScaling(&_transformData.ScaleMatrix, scale.x, scale.y, scale.z);
	flush();
}

void Transform::setLocalPosition(const Vector3& position)
{
	D3DXMatrixTranslation(&_transformData.PositionMatrix, position.x, position.y, position.z);
	flush();
}

void Transform::setLocalRotation(const Quaternion& rotation)
{
	D3DXMatrixRotationQuaternion(&_transformData.RotationMatrix, &rotation);
	flush();
}

void Transform::setLocalRotation(const Vector3& rotation)
{
	D3DXMatrixRotationYawPitchRoll(&_transformData.RotationMatrix, rotation.y, rotation.x, rotation.z);
}

void Transform::setPosition(const Vector3& position)
{
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &_transformData.CombTransformMatrix);

	Vector3 temp = t - position;
	D3DXMatrixTranslation(&_transformData.PositionMatrix, temp.x, temp.y, temp.z);
}

void Transform::setRotation(const Vector3& rotation)
{

}

void Transform::setRotation(const Quaternion& rotation)
{	
	Vector3 s, t;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &t, &_transformData.CombTransformMatrix);

	Quaternion temp = r - rotation;
	D3DXMatrixRotationQuaternion(&_transformData.RotationMatrix, &temp);
	flush();
}

void Transform::setParent(const Transform& parent)
{
}

Transform* Transform::getChild(int index)
{
	return nullptr;
}


void Transform::flush()
{
	_transformData.TransformMatrix = _transformData.ScaleMatrix * _transformData.RotationMatrix * _transformData.PositionMatrix;	
}