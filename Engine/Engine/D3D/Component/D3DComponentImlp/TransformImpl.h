#pragma once
#include "TransformImplBase.h"
//#include "..\Transform.h"

class Transform;

class TransformImpl : public TransformImplBase
{
	typedef std::vector<wp<Transform>> TransformVec;
public:
	TransformImpl();
	~TransformImpl();

	virtual void Update(IDirect3DDevice9* pDevice) override;
	virtual void UpdateMatrix() override;

	virtual void SetLocalScale(const Vector3& inScale) override;
	virtual void SetLocalRotation(const Quaternion& inRotation) override;
	virtual void SetLocalPosition(const Vector3& inPosition) override;

	virtual void GetLocalScale(Vector3& outLocalScale) const override;
	virtual void GetLocalRotation(Quaternion& outLocalRotation) const override;
	virtual void GetLocalEulerAngle(Vector3& outLocalEulerAngle) const override;
	virtual void GetLocalPosition(Vector3& outLocalPosition) const override;

	virtual void GetScale(Vector3& outScale) const override;
	virtual void GetRotation(Quaternion& outRotation) const override;
	virtual void GetEulerAngle(Vector3& outEulerAngle) const override;
	virtual void GetPosition(Vector3& outPosition) const override;

	virtual void GetAxisX(Vector3& outAxisX) const override;
	virtual void GetAxisY(Vector3& outAxisY) const override;
	virtual void GetAxisZ(Vector3& outAxisZ) const override;

	void GetMatrixLocal(D3DXMATRIX& outMatLocal) const;
	void GetMatrixWorldParent(D3DXMATRIX& outMatLocal) const;

	void SetParent(wp<Transform> pParent) const;
	uint GetChildCount() const;
	wp<Transform> GetParent() const;
	wp<Transform> GetChild(uint index) const;

private:
	void InternalAddChild(wp<Transform> pChild);
	void InternalRemoveChild(wp<Transform> pChild);

	void InternalSetScale(const Vector3& s);
	void InternalSetRotation(const Quaternion& q);
	void InternalSetTranslation(const Vector3& t);
	void InternalUpdateWorldMatrix();

	void InternalMatrixToScale(const D3DXMATRIX& matrix, Vector3& outScale) const;
	void InternalMatrixToRotation(const D3DXMATRIX& matrix, Quaternion& outRotation) const;
	void InternalMatrixToTranslation(const D3DXMATRIX& matrix, Vector3& outPosition) const;


private:
	sp<Vector3>		_pScale;
	sp<Quaternion>	_pRotation;
	sp<Vector3>		_pPosition;
	sp<Vector3>		_pAxisX;
	sp<Vector3>		_pAxisY;
	sp<Vector3>		_pAxisZ;

	wp<Transform>	_pParent;
	sp<TransformVec> _pChildrenVec;
};

inline void TransformImpl::GetLocalScale(Vector3& outLocalScale) const
{
	InternalMatrixToScale(*_matLocal, outLocalScale);
}

inline void TransformImpl::GetLocalRotation(Quaternion& outLocalRotation) const
{
	InternalMatrixToRotation(*_matLocal, outLocalRotation);
}

inline void TransformImpl::GetLocalEulerAngle(Vector3& outLocalEulerAngle) const
{
	Quaternion outLocalRotation;
	InternalMatrixToRotation(*_matLocal, outLocalRotation);
	outLocalEulerAngle =  Quaternion::ToEulerAngle(outLocalRotation);
}

inline void TransformImpl::GetLocalPosition(Vector3& outLocalPosition) const
{
	InternalMatrixToTranslation(*_matLocal, outLocalPosition);
}

inline void TransformImpl::GetScale(Vector3& outScale) const
{
	InternalMatrixToScale(*_matLocal * *_matWorldParent, outScale);
}

inline void TransformImpl::GetRotation(Quaternion& outRotation) const
{
	InternalMatrixToRotation(*_matLocal, outRotation);
}

inline void TransformImpl::GetEulerAngle(Vector3& outEulerAngle) const
{
	Quaternion outLocalRotation;
	InternalMatrixToRotation(*_matLocal * *_matWorldParent, outLocalRotation);
	outEulerAngle = Quaternion::ToEulerAngle(outLocalRotation);
}

inline void TransformImpl::GetPosition(Vector3& outPosition) const
{
	InternalMatrixToTranslation(*_matLocal * *_matWorldParent, outPosition);
}

inline void TransformImpl::GetAxisX(Vector3& outAxisX) const
{
	outAxisX = *_pAxisX;
}

inline void TransformImpl::GetAxisY(Vector3& outAxisY) const
{
	outAxisY = *_pAxisY;
}

inline void TransformImpl::GetAxisZ(Vector3& outAxisZ) const
{
	outAxisZ = *_pAxisZ;	
}

inline void TransformImpl::GetMatrixLocal(D3DXMATRIX& outMatLocal) const
{
	outMatLocal = *_matLocal;
}

inline void TransformImpl::GetMatrixWorldParent(D3DXMATRIX& outMatWorldParent) const
{
	outMatWorldParent = *_matWorldParent;
}

