#pragma once
#include "..\D3DDescription\D3DDescription.h"

class AnimationCurve
{
public:
	AnimationCurve();
	~AnimationCurve();

	// Unity ���� Animation Key �߰��ϴ� ��ó�� �� �� �ֵ��� ����
	void SetScaleKey(int frame, const Vector3& scale);
	void SetRotationKey(int frame, const  Quaternion& rotation);
	void SetPositionKey(int frame, const  Vector3& position);
	void SetBoneName(const String& name);
	void SetFrame(); // frame start, end, length ���ϱ�

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
