#pragma once
#include "..\D3DDescription\D3DDescription.h"

class AnimationCurve
{
public:
	AnimationCurve();
	~AnimationCurve();

	// Unity ���� Animation Key �߰��ϴ� ��ó�� �� �� �ֵ��� ����
	void SetScaleKey(int frame, const Vector3& scale);
	void SetRotationKey(int frame, const  Vector3& rotation);
	void SetPositionKey(int frame, const  Vector3& position);
	void SetBoneName(const String& name);
	void SetFrame(); // frame start, end, length ���ϱ�

	bool GetScaleKey(int frame, Vector3& scale) const;
	bool GetRotationKey(int frame, Vector3& rotation) const;
	bool GetPositionKey(int frame, Vector3& position) const;

	void GetAnimatedMatrix(int frame, D3DXMATRIX& matrix) const;
	void GetBoneName(String& name) const;
	int GetLength() const;

private:
	void SetKey(std::vector<ANIMATIONKEY>& keySetVec, int frame, const Vector3& value);
	bool GetKey(const std::vector<ANIMATIONKEY>& keySetVec, int frame, Vector3& value) const;
	

private:
	ANIMATIONKEYSET _animKeySet;
};
