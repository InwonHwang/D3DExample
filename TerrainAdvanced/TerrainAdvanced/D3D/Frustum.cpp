#include "Frustum.h"
#include "Device.h"
#include "Method\Transform.h"

Frustum::Frustum()
	: _vertice(nullptr),
	_planes(nullptr)
{
}


Frustum::~Frustum()
{
}

bool Frustum::create()
{
	assert(_vertice == nullptr && "vertice already created");
	assert(_planes == nullptr && "planes already created");

	_vertice = new Vector3[8];
	_planes = new D3DXPLANE[6];

	memset(_vertice, 0, sizeof(Vector3) * 8);
	memset(_planes, 0, sizeof(D3DXPLANE) * 6);

	return true;
}

void Frustum::setTransform(Transform& t)
{
	_t = &t;
}

bool Frustum::make(const D3DXMATRIXA16& viewProjection)
{	
	// Frustum이 카메라와 다르게 생성됨
	_vertice[0].x = -1.0f;	_vertice[0].y = -1.0f;	_vertice[0].z = 0.0f;
	_vertice[1].x =  1.0f;	_vertice[1].y = -1.0f;	_vertice[1].z = 0.0f;
	_vertice[2].x =  1.0f;	_vertice[2].y = -1.0f;	_vertice[2].z = 1.0f;
	_vertice[3].x = -1.0f;	_vertice[3].y = -1.0f;	_vertice[3].z = 1.0f;
	_vertice[4].x = -1.0f;	_vertice[4].y =  1.0f;	_vertice[4].z = 0.0f;
	_vertice[5].x =  1.0f;	_vertice[5].y =  1.0f;	_vertice[5].z = 0.0f;
	_vertice[6].x =  1.0f;	_vertice[6].y =  1.0f;	_vertice[6].z = 1.0f;
	_vertice[7].x = -1.0f;	_vertice[7].y =  1.0f;	_vertice[7].z = 1.0f;

	D3DXMATRIX matInv;
	D3DXMatrixInverse(&matInv, NULL, &viewProjection);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&_vertice[i], &_vertice[i], &matInv);
	}

	_camPos = (_vertice[0] + _vertice[5]) / 2.0f;
	
	/*D3DXMATRIX matView, matProj;
	D3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	for (int i = 0; i < 8; ++i)
		D3DXVec3Unproject(
			&_vertice[i],
			&_vertice[i],
			nullptr,
			&matProj,
			&matView,
			nullptr);*/

	//D3DXPlaneFromPoints(&_planes[0], _vertice+4, _vertice+7, _vertice+6);	// 상 평면(top)
	//D3DXPlaneFromPoints(&_planes[1], _vertice  , _vertice+1, _vertice+2);	// 하 평면(bottom)
	//D3DXPlaneFromPoints(&_planes[2], _vertice  , _vertice+4, _vertice+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&_planes[3], _vertice + 2, _vertice + 6, _vertice + 7);	// 원 평면(far)
	D3DXPlaneFromPoints(&_planes[4], _vertice, _vertice + 3, _vertice + 7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&_planes[5], _vertice + 1, _vertice + 5, _vertice + 6);	// 우 평면(right)
	
	

	return true;
}

void Frustum::destroy()
{
	SAFE_DELETE_ARRAY(_vertice);
	SAFE_DELETE_ARRAY(_planes);
}

bool Frustum::isIn(const Vector3& position) const
{
	assert(_planes != nullptr && "vertice is not created");

	for (int i = 3; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &position) > PLANE_EPSILON)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::isInSphere(const Vector3& position, float radius) const
{
	assert(_planes != nullptr && "planes is not created");

	for (int i = 3; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &position) > radius + PLANE_EPSILON)
		{
			return false;
		}
	}
	return true;
}

void Frustum::draw()
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

	D3DDevice->SetFVF(D3DFVF_XYZ);
	D3DDevice->SetStreamSource(0, NULL, 0, sizeof(VTX));
	D3DDevice->SetTexture(0, NULL);
	D3DDevice->SetIndices(0);

	// 파란색으로 상,하 평면을 그린다.
	//D3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	//D3DDevice->SetMaterial(&mtrl);
	//D3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	// 녹색으로 좌,우 평면을 그린다.
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	//D3DDevice->SetMaterial(&mtrl);
	D3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	// 붉은색으로 원,근 평면을 그린다.
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	//D3DDevice->SetMaterial(&mtrl);
	//D3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	
}
