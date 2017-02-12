#pragma once
#include "..\..\Core\Core.h"
#include "..\Resource\D3DResource.h"

class TransformData;

class TransformImpl
{
public:
	TransformImpl();
	~TransformImpl();

	// friend private 함수로 바꾸기
	virtual void Update(IDirect3DDevice9& device);
	void UpdateWorldMatrix();
	//

	void SetLocalScale(const Vector3& scale);
	void SetLocalRotation(const Quaternion& rotation);
	void SetLocalPosition(const Vector3& position);

	Vector3 GetLocalScale() const;
	Quaternion GetLocalRotation() const;
	Vector3 GetLocalEulerAngle() const;
	Vector3 GetLocalPosition() const;

	Vector3 GetScale() const;
	Quaternion GetRotation() const;
	Vector3 GetEulerAngle() const;
	Vector3 GetPosition() const;

	Vector3 GetAxisX() const;
	Vector3 GetAxisY() const;
	Vector3 GetAxisZ() const;

	// friend private 함수로 바꾸기
	D3DXMATRIX GetMatrix() const;

private:
	void InternalSetScale(const Vector3& s);
	void InternalSetRotation(const Quaternion& q);
	void InternalSetTranslation(const Vector3& t);
	void InternalUpdateWorldMatrix();

	Vector3 InternalMatrixToScale(const D3DXMATRIX& matrix) const;
	Quaternion InternalMatrixToRotation(const D3DXMATRIX& matrix) const;
	Vector3 InternalMatrixToTranslation(const D3DXMATRIX& matrix) const;


private:
	sp<TransformData> _transformData;

	Vector3*		_scale;
	Quaternion*		_rotation;
	Vector3*		_position;

	Vector3*		_axisX;
	Vector3*		_axisY;
	Vector3*		_axisZ;
};

inline Vector3 TransformImpl::GetLocalScale() const
{
	return *_scale;
}

inline Quaternion TransformImpl::GetLocalRotation() const
{
	return *_rotation;
}

inline Vector3 TransformImpl::GetLocalEulerAngle() const
{
	return Quaternion::ToEulerAngle(*_rotation);
}

inline Vector3 TransformImpl::GetLocalPosition() const
{
	return *_position;
}

inline Vector3 TransformImpl::GetScale() const
{
	return InternalMatrixToScale(_transformData->GetMatrix() * _transformData->GetMatrixParent());
}

inline Quaternion TransformImpl::GetRotation() const
{
	return InternalMatrixToRotation(_transformData->GetMatrix() * _transformData->GetMatrixParent());
}

inline Vector3 TransformImpl::GetEulerAngle() const
{
	return Quaternion::ToEulerAngle(InternalMatrixToRotation(_transformData->GetMatrix() * _transformData->GetMatrixParent()));
}

inline Vector3 TransformImpl::GetPosition() const
{
	return InternalMatrixToTranslation(_transformData->GetMatrix() * _transformData->GetMatrixParent());
}

inline Vector3 TransformImpl::GetAxisX() const
{
	return *_axisX;
}

inline Vector3 TransformImpl::GetAxisY() const
{
	return *_axisY;
}

inline Vector3 TransformImpl::GetAxisZ() const
{
	return *_axisZ;
}

inline D3DXMATRIX TransformImpl::GetMatrix() const
{
	return _transformData->GetMatrixParent() * _transformData->GetMatrix();
}

