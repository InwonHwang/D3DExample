#include "IWETransform.h"
#include "IWEGameObject.h"
#include "Device.h"

Transform::Transform()
	:
	_position(0, 0, 0),
	_rotation(0, 0, 0, 0),
	_scale(1, 1, 1),
	_localPosition(_position),
	_localRotation(_rotation),
	_localScale(_scale)	
{	
	/*D3DXMatrixScaling(&_matScale, _scale.x(), _scale.y(), _scale.z());
	D3DXMatrixTranslation(&_matPosition, _position.x(), _position.y(), _position.z());
	D3DXMatrixRotationQuaternion(&_matRotation, &_rotation._d3dxQuaternion);*/
	D3DXMatrixScaling(&_matScale, _scale.x, _scale.y, _scale.z);
	D3DXMatrixTranslation(&_matPosition, _position.x, _position.y, _position.z);
	D3DXMatrixRotationQuaternion(&_matRotation, &_rotation);
}

Transform::~Transform()
{	
}

void Transform::setPosition(const IWEVector3& pos)
{
	_position = pos;

	if (_parent)
		_localPosition = _position - _parent->getPosition();
	else
		_localPosition = _position;

	//D3DXMatrixTranslation(&_matPosition, _position.x(), _position.y(), _position.z());
	D3DXMatrixTranslation(&_matPosition, _position.x, _position.y, _position.z);
}
void Transform::setLocalPosition(const IWEVector3& localPos)
{
	_localPosition = localPos;

	if (_parent)
		_position = _localPosition + _parent->getPosition();
	else
		_position = _localPosition;

	//D3DXMatrixTranslation(&_matPosition, _position.x(), _position.y(), _position.z());
	D3DXMatrixTranslation(&_matPosition, _position.x, _position.y, _position.z);
}
void Transform::setRotation(const IWEQuaternion& rot)
{	
	_rotation = rot;

	if (_parent)
		_localRotation = _rotation - _parent->getRotation();
	else
		_localRotation = _rotation;
			
	D3DXMatrixRotationQuaternion(&_matRotation, &_rotation);
}
void Transform::setLocalRotation(const IWEQuaternion& localRot)
{
	_localRotation = localRot;

	if (_parent)
		_rotation = _localRotation + _parent->getRotation();
	else
		_rotation = _localRotation;

	D3DXMatrixRotationQuaternion(&_matRotation, &_rotation);
}
void Transform::setScale(const IWEVector3& scale)
{
	_scale = scale;

	if (_parent)
		_localScale = _scale - _parent->getScale();
	else
		_localScale = _scale;

	D3DXMatrixScaling(&_matScale, _scale.x, _scale.y, _scale.z);
}
void Transform::setLocalScale(const IWEVector3& localScale)
{
	_localScale = localScale;

	if (_parent)
		_scale = _localScale + _parent->getScale();
	else
		_scale = _localScale;

	//D3DXMatrixScaling(&_matScale, _scale.x(), _scale.y(), _scale.z());
	D3DXMatrixScaling(&_matScale, _scale.x, _scale.y, _scale.z);
}

void Transform::update()
{	
	_world = _matScale * _matRotation * _matPosition;

	g_Device->SetTransform(D3DTS_WORLD, &(_world));

}
