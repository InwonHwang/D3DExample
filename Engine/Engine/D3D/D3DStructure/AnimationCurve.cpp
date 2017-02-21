#include "AnimationCurve.h"



AnimationCurve::AnimationCurve()
{
}


AnimationCurve::~AnimationCurve()
{
}

void AnimationCurve::SetBoneName(const String& name)
{
	_animKeySet.name = name;
}

void AnimationCurve::SetFrame()
{
	_animKeySet.start = _animKeySet.scale.front().frame;
	_animKeySet.end = _animKeySet.scale.back().frame;
	_animKeySet.length = _animKeySet.end - _animKeySet.start;	
}

void AnimationCurve::SetScaleKey(int frame, const Vector3& scale)
{
	SetKey(_animKeySet.scale, frame, scale);
}
void AnimationCurve::SetRotationKey(int frame, const Quaternion& rotation)
{
	SetKey(_animKeySet.rotation, frame, rotation);
}
void AnimationCurve::SetPositionKey(int frame, const Vector3& position)
{
	SetKey(_animKeySet.position, frame, position);
}

bool AnimationCurve::GetScaleKey(int frame, Vector3& scale) const
{
	return GetKey(_animKeySet.scale, frame, scale);
}
bool AnimationCurve::GetRotationKey(int frame, Quaternion& rotation) const
{
	return GetKey(_animKeySet.rotation, frame, rotation);
}
bool AnimationCurve::GetPositionKey(int frame, Vector3& position) const
{
	return GetKey(_animKeySet.position, frame, position);
}

void AnimationCurve::GetAnimatedMatrix(int frame, D3DXMATRIX& matrix) const
{
	Vector3 v(0, 0, 0);
	Quaternion q(0, 0, 0, 0);

	//D3DXMatrixIdentity(&matrix);

	if (GetRotationKey(frame, q)) {
		D3DXMatrixRotationQuaternion(&matrix, &q);
	}

	if (GetPositionKey(frame, v)) {
		matrix._41 = v.x;
		matrix._42 = v.y;
		matrix._43 = v.z;
	}

	if (GetScaleKey(frame, v)) {
		matrix._11 *= v.x;
		matrix._22 *= v.y;
		matrix._33 *= v.z;
	}
}
void AnimationCurve::GetBoneName(String& name) const
{
	name = _animKeySet.name;
}
int AnimationCurve::GetLength() const
{
	return _animKeySet.length;
}

void AnimationCurve::SetKey(std::vector<VECTORKEY>& keySetVec, int frame, const Vector3& value)
{
	VECTORKEY tempKey;
	tempKey.frame = frame;
	tempKey.value = value;

	auto comepare = [](const VECTORKEY& lhs, const VECTORKEY& rhs) {
		return lhs.frame < rhs.frame;
	};

	auto it = std::lower_bound(keySetVec.begin(), keySetVec.end(), tempKey, comepare);
	keySetVec.insert(it, tempKey);
}

bool AnimationCurve::GetKey(const std::vector<VECTORKEY>& keySetVec, int frame, Vector3& value) const
{
	if (keySetVec.size() == 0 ||					// 안에 키가 없을 때
		frame < keySetVec.front().frame ||			// frame값이 keySet 안의 
		frame > keySetVec.back().frame)			// 프레임 값들의 범위를 벗어날 때
		return false;
		

	int index = 0;
	while (keySetVec[index].frame < frame)
		index++;

	if (index == 0)
	{
		value = keySetVec[index].value;
	}
	else
	{
		float t = Mathf::Lerp(float(keySetVec[index - 1].frame), float(keySetVec[index].frame), (float)frame);		
		D3DXVec3Lerp(&value, &keySetVec[index - 1].value, &keySetVec[index].value, t);		
	}

	return true;
}

void AnimationCurve::SetKey(std::vector<QUATERNIONKEY>& keySetVec, int frame, const Quaternion& value)
{
	QUATERNIONKEY tempKey;
	tempKey.frame = frame;
	tempKey.value = value;

	auto comepare = [](const QUATERNIONKEY& lhs, const QUATERNIONKEY& rhs) {
		return lhs.frame < rhs.frame;
	};

	auto it = std::lower_bound(keySetVec.begin(), keySetVec.end(), tempKey, comepare);
	keySetVec.insert(it, tempKey);
}

bool AnimationCurve::GetKey(const std::vector<QUATERNIONKEY>& keySetVec, int frame, Quaternion& value) const
{
	if (keySetVec.size() == 0 ||					// 안에 키가 없을 때
		frame < keySetVec.front().frame ||			// frame값이 keySet 안의 
		frame > keySetVec.back().frame)			// 프레임 값들의 범위를 벗어날 때
		return false;


	int index = 0;
	while (keySetVec[index].frame < frame)
		index++;

	if (index == 0)
	{
		value = keySetVec[index].value;
	}
	else
	{
		float t = Mathf::Lerp(float(keySetVec[index - 1].frame), float(keySetVec[index].frame), (float)frame);
		D3DXQuaternionSlerp(&value, &keySetVec[index - 1].value, &keySetVec[index].value, t);
	}

	return true;
}