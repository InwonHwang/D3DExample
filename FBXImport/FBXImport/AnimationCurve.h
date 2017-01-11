#pragma once
#include "Core.h"

typedef struct tagScaleKey
{
	float Frame;
	D3DXVECTOR3 Value;
}SCALEKEY, *LPSCALEKEY;

typedef struct tagRotationKey
{
	float Frame;
	D3DXQUATERNION Value;
}ROTATIONKEY, *LPROTATIONKEY;

typedef struct tagPositionKey
{
	float Frame;
	D3DXVECTOR3 Value;
}POSITIONKEY, *LPPOSITIONKEY;

// �ִϸ��̼� Ű ���� ���� �ִ� Ŭ����
class AnimationCurve
{
public:
	AnimationCurve() {}
	~AnimationCurve() {}

	// Unity ���� Animation Key �߰��ϴ� ��ó�� �� �� �ֵ��� ����
	void SetScaleKey(float frame, D3DXVECTOR3& scale);
	void SetRotationKey(float frame, D3DXQUATERNION& rotation);
	void SetPositionKey(float frame, D3DXVECTOR3& position);

	bool GetScaleKey(float frame, D3DXVECTOR3& scale) const;
	bool GetRotationKey(float frame, D3DXQUATERNION& rotation) const;
	bool GetPositionKey(float frame, D3DXVECTOR3& position) const;

	D3DXMATRIX* GetAnimatedMatrix(float frame);

	//Mathf �Լ��� �߰��ϱ�
	float Lerp(float f1, float f2, float frame) const;

private:
	float time;
	std::vector<SCALEKEY>		_keyScale;
	std::vector<ROTATIONKEY>	_keyRotation;
	std::vector<POSITIONKEY>	_keyPosition;
	D3DXMATRIX					_matAnim;
};



inline float AnimationCurve::Lerp(float f1, float f2, float frame) const
{
	return ((frame - f1) / (f2 - f1));
}

