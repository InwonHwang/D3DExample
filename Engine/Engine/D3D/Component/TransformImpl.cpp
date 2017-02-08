#include "TransformImpl.h"

extern ResourceManager resourceManager;

TransformImpl::TransformImpl()
{
	_scale = new Vector3(1.0f, 1.0f, 1.0f);
	_rotation = new Quaternion(Quaternion::Euler(0.0f, 0.0f, 0.0f));	
	_position = new Vector3(0.0f, 0.0f, 0.0f);
	_axisX = new Vector3(1.0f, 0.0f, 0.0f);
	_axisY = new Vector3(0.0f, 1.0f, 0.0f);
	_axisZ = new Vector3(0.0f, 0.0f, 1.0f);
	_transformData = resourceManager.Create<TransformData>();
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

void TransformImpl::Update(IDirect3DDevice9& device)
{
	D3DXMATRIX matWorld = _transformData->GetMatrix() * _transformData->GetMatrixParent();
	device.SetTransform(D3DTS_WORLD, &matWorld);	
}

void TransformImpl::UpdateWorldMatrix()
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
	D3DXMATRIX result;
	D3DXMatrixScaling(&scale, _scale->x, _scale->y, _scale->z);
	D3DXMatrixRotationQuaternion(&rotation, _rotation);
	D3DXMatrixTranslation(&translation, _position->x, _position->y, _position->z);
	result = scale * rotation * translation;

	_transformData->SetMatrix(result);	
}

Vector3 TransformImpl::InternalMatrixToScale(const D3DXMATRIX& matrix) const
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return s;
}

Quaternion TransformImpl::InternalMatrixToRotation(const D3DXMATRIX& matrix) const
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return r;
}

Vector3 TransformImpl::InternalMatrixToTranslation(const D3DXMATRIX& matrix) const
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
	D3DXMatrixDecompose(&s, &r, &t, &matrix);

	return t;
}
