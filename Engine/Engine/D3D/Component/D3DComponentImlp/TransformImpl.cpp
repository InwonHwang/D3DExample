#include "TransformImpl.h"

extern ResourceManager resourceManager;

TransformImpl::TransformImpl()
{
	D3DXMatrixIdentity(&_matLocal);
	D3DXMatrixIdentity(&_matWorldParent);
	_scale = new Vector3(1.0f, 1.0f, 1.0f);
	_rotation = new Quaternion(Quaternion::Euler(0.0f, 0.0f, 0.0f));	
	_position = new Vector3(0.0f, 0.0f, 0.0f);
	_axisX = new Vector3(1.0f, 0.0f, 0.0f);
	_axisY = new Vector3(0.0f, 1.0f, 0.0f);
	_axisZ = new Vector3(0.0f, 0.0f, 1.0f);
}


TransformImpl::~TransformImpl()
{
	SafeDelete<Vector3>(_scale);
	SafeDelete<Quaternion>(_rotation);
	SafeDelete<Vector3>(_position);
	SafeDelete<Vector3>(_axisX);
	SafeDelete<Vector3>(_axisY);
	SafeDelete<Vector3>(_axisZ);
}

void TransformImpl::Update(IDirect3DDevice9* pDevice)
{
	assert(pDevice);

	D3DXMATRIX matWorld = _matLocal * _matWorldParent;
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
	*_scale = s;
}

void TransformImpl::InternalSetRotation(const Quaternion& q)
{
	*_rotation = q;

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &q);

	D3DXVec3TransformCoord(_axisX, &Vector3(1.0f, 0.0f, 0.0f), &matRot);
	D3DXVec3TransformCoord(_axisY, &Vector3(0.0f, 1.0f, 0.0f), &matRot);
	D3DXVec3TransformCoord(_axisZ, &Vector3(0.0f, 0.0f, 1.0f), &matRot);
	D3DXVec3Normalize(_axisX, _axisX);
	D3DXVec3Normalize(_axisY, _axisY);
	D3DXVec3Normalize(_axisZ, _axisZ);
}

void TransformImpl::InternalSetTranslation(const Vector3& t)
{
	*_position = t;
}

void TransformImpl::InternalUpdateWorldMatrix()
{
	D3DXMATRIX scale, rotation , translation;
	
	D3DXMatrixScaling(&scale, _scale->x, _scale->y, _scale->z);
	D3DXMatrixRotationQuaternion(&rotation, _rotation);
	D3DXMatrixTranslation(&translation, _position->x, _position->y, _position->z);

	_matLocal = scale * rotation * translation;	
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
