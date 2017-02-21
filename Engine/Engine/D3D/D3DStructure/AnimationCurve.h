#pragma once
#include "..\D3DDescription\D3DDescription.h"

class AnimationCurve
{
public:
	AnimationCurve();
	~AnimationCurve();

	// Unity 에서 Animation Key 추가하는 것처럼 할 수 있도록 제공
	void SetScaleKey(int frame, const Vector3& scale);
	void SetRotationKey(int frame, const  Quaternion& rotation);
	void SetPositionKey(int frame, const  Vector3& position);
	void SetBoneName(const String& name);
	void SetFrame(); // frame start, end, length 구하기

	bool GetScaleKey(int frame, Vector3& scale) const;
	bool GetRotationKey(int frame, Quaternion& rotation) const;
	bool GetPositionKey(int frame, Vector3& position) const;

	void GetAnimatedMatrix(int frame, D3DXMATRIX& matrix) const;
	void GetBoneName(String& name) const;
	int GetLength() const;

private:
	void SetKey(std::vector<VECTORKEY>& keySetVec, int frame, const Vector3& value);
	void SetKey(std::vector<QUATERNIONKEY>& keySetVec, int frame, const Quaternion& value);
	bool GetKey(const std::vector<VECTORKEY>& keySetVec, int frame, Vector3& value) const;
	bool GetKey(const std::vector<QUATERNIONKEY>& keySetVec, int frame, Quaternion& value) const;

private:
	ANIMATIONKEYSET _animKeySet;
};
