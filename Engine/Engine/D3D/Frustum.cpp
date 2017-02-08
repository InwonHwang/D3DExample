#include "Frustum.h"

Frustum::Frustum()
{
	_vertice = new Vector3[8];
	_planes = new D3DXPLANE[6];
}


Frustum::~Frustum()
{
	SafeDeleteArray<Vector3>(_vertice);
	SafeDeleteArray<D3DXPLANE>(_planes);
}

bool Frustum::Make(const D3DXMATRIXA16& matViewProj)
{
	assert(_planes != nullptr && "vertice are not created");

	_vertice[0].x = -1.0f;	_vertice[0].y = -1.0f;	_vertice[0].z = 0.0f;
	_vertice[1].x = 1.0f;	_vertice[1].y = -1.0f;	_vertice[1].z = 0.0f;
	_vertice[2].x = 1.0f;	_vertice[2].y = -1.0f;	_vertice[2].z = 1.0f;
	_vertice[3].x = -1.0f;	_vertice[3].y = -1.0f;	_vertice[3].z = 1.0f;
	_vertice[4].x = -1.0f;	_vertice[4].y = 1.0f;	_vertice[4].z = 0.0f;
	_vertice[5].x = 1.0f;	_vertice[5].y = 1.0f;	_vertice[5].z = 0.0f;
	_vertice[6].x = 1.0f;	_vertice[6].y = 1.0f;	_vertice[6].z = 1.0f;
	_vertice[7].x = -1.0f;	_vertice[7].y = 1.0f;	_vertice[7].z = 1.0f;

	D3DXMATRIX matInv;
	D3DXMatrixInverse(&matInv, NULL, &matViewProj);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&_vertice[i], &_vertice[i], &matInv);
	}

	D3DXPlaneFromPoints(&_planes[0], _vertice+4, _vertice+7, _vertice+6);	// 상 평면(top)
	D3DXPlaneFromPoints(&_planes[1], _vertice  , _vertice+1, _vertice+2);	// 하 평면(bottom)
	D3DXPlaneFromPoints(&_planes[2], _vertice  , _vertice+4, _vertice+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&_planes[3], _vertice + 2, _vertice + 6, _vertice + 7);	// 원 평면(far)
	D3DXPlaneFromPoints(&_planes[4], _vertice, _vertice + 3, _vertice + 7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&_planes[5], _vertice + 1, _vertice + 5, _vertice + 6);	// 우 평면(right)

	return true;
}

bool Frustum::IsIn(const Vector3& position) const
{
	assert(_planes != nullptr && "plane is not created");

	for (int i = 3; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &position) > PLANE_EPSILON)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::IsInSphere(const Vector3& position, float radius) const
{
	assert(_planes != nullptr && "plane is not created");

	for (int i = 3; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &position) > radius + PLANE_EPSILON)
		{
			return false;
		}
	}
	return true;
}

void Frustum::Draw(IDirect3DDevice9& device)
{
	WORD		index[] = { 0, 1, 2,
		0, 2, 3,
		4, 7, 6,
		4, 6, 5,
		1, 5, 6,
		1, 6, 2,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		3, 7, 6,
		3, 6, 2 };

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	typedef struct tagVTX
	{
		D3DXVECTOR3	p;
	} VTX;

	VTX		vtx[8];

	for (int i = 0; i < 8; i++)
		vtx[i].p = _vertice[i];

	device.SetFVF(D3DFVF_XYZ);
	device.SetStreamSource(0, NULL, 0, sizeof(VTX));
	device.SetTexture(0, NULL);
	device.SetIndices(0);

	// 파란색으로 상,하 평면을 그린다.
	device.SetRenderState(D3DRS_LIGHTING, true);
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	device.SetMaterial(&mtrl);
	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	// 녹색으로 좌,우 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	device.SetMaterial(&mtrl);
	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	// 붉은색으로 원,근 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	device.SetMaterial(&mtrl);
	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
}
