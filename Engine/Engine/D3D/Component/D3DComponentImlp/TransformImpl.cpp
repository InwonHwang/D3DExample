#include "TransformImpl.h"
#include "..\Transform.h"

TransformImpl::TransformImpl()
	: TransformImplBase()
{
	_pScale.reset(new Vector3(1.0f, 1.0f, 1.0f));
	_pRotation.reset(new Quaternion(0, 0, 0, 0));
	_pPosition.reset(new Vector3(0.0f, 0.0f, 0.0f));
	_pAxisX.reset(new Vector3(1.0f, 0.0f, 0.0f));
	_pAxisY.reset(new Vector3(0.0f, 1.0f, 0.0f));
	_pAxisZ.reset(new Vector3(0.0f, 0.0f, 1.0f));
	
	_pChildrenVec.reset(new TransformVec);
}


TransformImpl::~TransformImpl()
{
	_pScale.reset();
	_pRotation.reset();
	_pPosition.reset();
	_pAxisX.reset();
	_pAxisY.reset();
	_pAxisZ.reset();

	_pChildrenVec.reset();
}

void TransformImpl::Update(IDirect3DDevice9* pDevice)
{
	assert(pDevice);

	if (!_pParent.expired())
	{
		_pParent.lock()->GetMatrixWorld(*_matWorldParent);
		_matWorldParent->_11 = 1.0f;
		_matWorldParent->_22 = 1.0f;
		_matWorldParent->_33 = 1.0f;
	}

	D3DXMATRIX matWorld = *_matLocal * *_matWorldParent;
	pDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void TransformImpl::UpdateMatrix()
{
	InternalUpdateWorldMatrix();
}

void TransformImpl::SetLocalScale(const Vector3& scale)
{
	InternalSetScale(scale);
}

void TransformImpl::SetLocalRotation(const Quaternion& q)
{
	InternalSetRotation(q);
}

void TransformImpl::SetLocalPosition(const Vector3& position)
{
	InternalSetTranslation(position);
}

void TransformImpl::InternalSetScale(const Vector3& s)
{
	*_pScale = s;
}

void TransformImpl::InternalSetRotation(const Quaternion& q)
{
	*_pRotation = q;

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &q);

	D3DXVec3TransformCoord(_pAxisX.get(), &Vector3(1.0f, 0.0f, 0.0f), &matRot);
	D3DXVec3TransformCoord(_pAxisY.get(), &Vector3(0.0f, 1.0f, 0.0f), &matRot);
	D3DXVec3TransformCoord(_pAxisZ.get(), &Vector3(0.0f, 0.0f, 1.0f), &matRot);
	D3DXVec3Normalize(_pAxisX.get(), _pAxisX.get());
	D3DXVec3Normalize(_pAxisY.get(), _pAxisY.get());
	D3DXVec3Normalize(_pAxisZ.get(), _pAxisZ.get());
}

void TransformImpl::InternalSetTranslation(const Vector3& t)
{
	*_pPosition = t;
}

void TransformImpl::InternalUpdateWorldMatrix()
{
	D3DXMATRIX scale, rotation , translation;
	
	D3DXMatrixScaling(&scale, _pScale->x, _pScale->y, _pScale->z);
	D3DXMatrixRotationQuaternion(&rotation, _pRotation.get());
	D3DXMatrixTranslation(&translation, _pPosition->x, _pPosition->y, _pPosition->z);

	*_matLocal = scale * rotation * translation;	
}

void TransformImpl::InternalMatrixToScale(const D3DXMATRIX& matrix, Vector3& outScale) const
{	
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&outScale, &r, &t, &matrix);
}

void TransformImpl::InternalMatrixToRotation(const D3DXMATRIX& matrix, Quaternion& outRotation) const
{
	Vector3 s;
	Vector3 t;
	D3DXMatrixDecompose(&s, &outRotation, &t, &matrix);
}

void TransformImpl::InternalMatrixToTranslation(const D3DXMATRIX& matrix, Vector3& outPosition) const
{
	Vector3 s;
	Quaternion r;
	D3DXMatrixDecompose(&s, &r, &outPosition, &matrix);
}

void TransformImpl::SetParent(sp<Transform> pParent, sp<Transform> pTransform)
{
	assert(pTransform);
	// pParent가 nullptr일때 sceneNode를 부모로 할 예정.
	
	//wp<Transform> pThis = pTransform;
	
	if (!_pParent.expired())
	{		
		_pParent.lock()->_impl->InternalRemoveChild(pTransform);		
	}
	
	_pParent = pParent;
	_pParent.lock()->_impl->InternalAddChild(pTransform);
}

void TransformImpl::InternalAddChild(wp<Transform> pChild)
{
	assert(_pChildrenVec);

	_pChildrenVec->push_back(pChild);
}
void TransformImpl::InternalRemoveChild(wp<Transform> pChild)
{
	assert(_pChildrenVec);

	for (auto it = _pChildrenVec->begin(); it != _pChildrenVec->end(); ++it)
	{		
		if (!it->expired() && it->lock() == pChild.lock())
		{
			_pChildrenVec->erase(it);
			break;
		}
	}
}
