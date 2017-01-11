#include "AnimationCurve.h"

void AnimationCurve::SetScaleKey(float frame, D3DXVECTOR3& scale)
{
	SCALEKEY scaleKey;
	scaleKey.Frame = frame;
	scaleKey.Value = scale;
	
	auto ramda = [](SCALEKEY lhs, SCALEKEY rhs) {
		return lhs.Frame < rhs.Frame;
	};
	auto it = std::lower_bound(_keyScale.begin(), _keyScale.end(), scaleKey, ramda);
								
	_keyScale.insert(it, scaleKey);
}

void AnimationCurve::SetRotationKey(float frame, D3DXQUATERNION& rotation)
{
	ROTATIONKEY rotationKey;
	rotationKey.Frame = frame;
	rotationKey.Value = rotation;

	auto ramda = [](ROTATIONKEY lhs, ROTATIONKEY rhs) {
		return lhs.Frame < rhs.Frame;
	};
	auto it = std::lower_bound(_keyRotation.begin(), _keyRotation.end(), rotationKey, ramda);

	_keyRotation.insert(it, rotationKey);
}

void AnimationCurve::SetPositionKey(float frame, D3DXVECTOR3& position)
{
	POSITIONKEY positionKey;
	positionKey.Frame = frame;
	positionKey.Value = position;

	auto ramda = [](POSITIONKEY lhs, POSITIONKEY rhs) {
		return lhs.Frame < rhs.Frame;
	};
	auto it = std::lower_bound(_keyPosition.begin(), _keyPosition.end(), positionKey, ramda);

	_keyPosition.insert(it, positionKey);
}

bool AnimationCurve::GetScaleKey(float frame, D3DXVECTOR3& scale) const
{
	if (_keyScale.size() == 0) return false;


	if (frame < _keyScale.front().Frame || frame > _keyScale.back().Frame)
		return false;

	int index = 0;
	while (_keyScale[index].Frame < frame) index++;

	if (index == 0)
	{
		scale = _keyScale[index].Value;
	}
	else
	{		
		float t = Lerp(_keyScale[index - 1].Frame, _keyScale[index].Frame, frame);
		D3DXVec3Lerp(&scale, &_keyScale[index - 1].Value, &_keyScale[index].Value, t);
	}

	return true;
}
	

bool AnimationCurve::GetRotationKey(float frame, D3DXQUATERNION& rotation) const
{
	if (_keyRotation.size() == 0) return false;


	if (frame < _keyRotation.front().Frame || frame > _keyRotation.back().Frame)
		return false;

	int index = 0;
	while (_keyRotation[index].Frame < frame) index++;

	if (index == 0)
	{
		rotation = _keyRotation[index].Value;
	}
	else
	{
		float t = Lerp(_keyRotation[index - 1].Frame, _keyRotation[index].Frame, frame);
		D3DXQuaternionSlerp(&rotation, &_keyRotation[index - 1].Value, &_keyRotation[index].Value, t);
	}

	return true;
}

bool AnimationCurve::GetPositionKey(float frame, D3DXVECTOR3& position) const
{
	if (_keyPosition.size() == 0) return false;


	if (frame < _keyPosition.front().Frame || frame > _keyPosition.back().Frame)
		return false;

	int index = 0;
	while (_keyPosition[index].Frame < frame) index++;

	if (index == 0)
	{
		position = _keyPosition[index].Value;
	}
	else
	{
		float t = Lerp(_keyPosition[index - 1].Frame, _keyPosition[index].Frame, frame);
		D3DXVec3Lerp(&position, &_keyPosition[index - 1].Value, &_keyPosition[index].Value, t);
	}

	return true;
}

D3DXMATRIX* AnimationCurve::GetAnimatedMatrix(float frame)
{
	D3DXVECTOR3 v(0, 0, 0);
	D3DXQUATERNION q;

	D3DXMatrixIdentity(&_matAnim);

	if (GetRotationKey(frame, q)) {
		D3DXMatrixRotationQuaternion(&_matAnim, &q);
	}

	if (GetPositionKey(frame, v)) {
		_matAnim._41 = v.x;
		_matAnim._42 = v.y;
		_matAnim._43 = v.z;
	}

	if (GetScaleKey(frame, v)) {
		_matAnim._11 *= v.x;
		_matAnim._22 *= v.y;
		_matAnim._33 *= v.z;
	}

	return &_matAnim;
}