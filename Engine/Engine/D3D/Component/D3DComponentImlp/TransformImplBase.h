#pragma once

#include "ComponentImpl.h"

class TransformImplBase : public ComponentImpl
{
	//friend class ComponentImpl;
public:
	TransformImplBase()
	{
		_matLocal.reset(new D3DXMATRIX);
		_matWorldParent.reset(new D3DXMATRIX);
		D3DXMatrixIdentity(_matLocal.get());
		D3DXMatrixIdentity(_matWorldParent.get());
	}
	virtual ~TransformImplBase()
	{
		_matLocal.reset();
		_matWorldParent.reset();
	}

	virtual void Update(IDirect3DDevice9* device) abstract;
	virtual void UpdateMatrix() abstract;

	virtual void SetLocalScale(const Vector3& inScale) abstract;
	virtual void SetLocalRotation(const Quaternion& inRotation) abstract;
	virtual void SetLocalPosition(const Vector3& inPosition) abstract;

	virtual void GetLocalScale(Vector3& outLocalScale) const abstract;
	virtual void GetLocalRotation(Quaternion& outLocalRotation) const abstract;
	virtual void GetLocalEulerAngle(Vector3& outLocalEulerAngle) const abstract;
	virtual void GetLocalPosition(Vector3& outLocalPosition) const abstract;

	virtual void GetScale(Vector3& outScale) const abstract;
	virtual void GetRotation(Quaternion& outRotation) const abstract;
	virtual void GetEulerAngle(Vector3& outEulerAngle) const abstract;
	virtual void GetPosition(Vector3& outPosition) const abstract;

	virtual void GetAxisX(Vector3& outAxisX) const abstract;
	virtual void GetAxisY(Vector3& outAxisY) const abstract;
	virtual void GetAxisZ(Vector3& outAxisZ) const abstract;
	
protected:
	sp<D3DXMATRIX> _matLocal;
	sp<D3DXMATRIX> _matWorldParent;
};

