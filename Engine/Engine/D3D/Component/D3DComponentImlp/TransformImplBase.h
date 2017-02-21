#pragma once

#include "..\..\..\Core\Core.h"
#include "ComponentImpl.h"

class TransformImplBase : public ComponentImpl
{
	//friend class ComponentImpl;
	friend class Transform;
public:
	virtual ~TransformImplBase() {}

	virtual void Update(IDirect3DDevice9* device) = 0;
	virtual void UpdateMatrix() = 0;

	virtual void SetLocalScale(const Vector3& inScale) = 0;
	virtual void SetLocalRotation(const Quaternion& inRotation) = 0;
	virtual void SetLocalPosition(const Vector3& inPosition) = 0;

	virtual void GetLocalScale(Vector3& outLocalScale) const = 0;
	virtual void GetLocalRotation(Quaternion& outLocalRotation) const = 0;
	virtual void GetLocalEulerAngle(Vector3& outLocalEulerAngle) const = 0;
	virtual void GetLocalPosition(Vector3& outLocalPosition) const = 0;

	virtual void GetScale(Vector3& outScale) const = 0;
	virtual void GetRotation(Quaternion& outRotation) const = 0;
	virtual void GetEulerAngle(Vector3& outEulerAngle) const = 0;
	virtual void GetPosition(Vector3& outPosition) const = 0;

	virtual void GetAxisX(Vector3& outAxisX) const = 0;
	virtual void GetAxisY(Vector3& outAxisY) const = 0;
	virtual void GetAxisZ(Vector3& outAxisZ) const = 0;
	
protected:
	D3DXMATRIX _matLocal;
	D3DXMATRIX _matWorldParent;
};

