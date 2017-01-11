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

// 애니메이션 키 값을 갖고 있는 클래스
class AnimationCurve
{
public:
	AnimationCurve() {}
	~AnimationCurve() {}

	// Unity 에서 Animation Key 추가하는 것처럼 할 수 있도록 제공
	void SetScaleKey(float frame, D3DXVECTOR3& scale);
	void SetRotationKey(float frame, D3DXQUATERNION& rotation);
	void SetPositionKey(float frame, D3DXVECTOR3& position);

	bool GetScaleKey(float frame, D3DXVECTOR3& scale) const;
	bool GetRotationKey(float frame, D3DXQUATERNION& rotation) const;
	bool GetPositionKey(float frame, D3DXVECTOR3& position) const;

	D3DXMATRIX* GetAnimatedMatrix(float frame);

	//Mathf 함수에 추가하기
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

