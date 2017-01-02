#include "Terrain.h"
#include "../Device.h"
#include "../Frustum.h"
#include <fstream>



Terrain::Terrain(UINT cellSpacing, float heightScale)
	: _cellSpacing(cellSpacing),
	_heightScale(heightScale)
{

}

Terrain::~Terrain()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}

void Terrain::draw(IDirect3DDevice9* device)
{
	device->SetStreamSource(0, _vb, 0, sizeof(TERRAINVERTEX));
	device->SetFVF(TERRAINVERTEX::FVF);
	device->SetIndices(_ib);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _numVertsCol*_numVertsRow, 0, _numTriangles);
}

bool Terrain::processFrustumCulling(const Frustum& f)
{
	WORD		i[4];	// �ӽ÷� ������ �ε��� ����
	BOOL		b[4];	// �ӽ÷� ������ frustum culling�����
	INDEX		idx;
	INDEX*		pI;

	if (FAILED(_ib->Lock(0, (_numVertsCol - 1)*(_numVertsRow - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
		return false;

	_numTriangles = 0;

	for (WORD z = 0; z < _numVertsRow - 1; z++)
	{
		for (WORD x = 0; x < _numVertsCol - 1; x++)
		{
			i[0] = (z*_numVertsCol + x);			// ���� ���
			i[1] = (z*_numVertsCol + x + 1);		// ���� ���
			i[2] = ((z + 1)*_numVertsCol + x);		// ���� �ϴ�
			i[3] = ((z + 1)*_numVertsCol + x + 1);	// ���� �ϴ�

			b[0] = f.isIn(_heightMap[i[0]]);	// ������� ������ Frustum�ȿ� �ִ°�?
			b[1] = f.isIn(_heightMap[i[1]]);	// ������� ������ Frustum�ȿ� �ִ°�?
			b[2] = f.isIn(_heightMap[i[2]]);	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
			if (b[0] | b[1] | b[2])				// ���߿� �ϳ��� frustum�ȿ� ������ �������Ѵ�.
			{
				idx._0 = i[0];
				idx._1 = i[1];
				idx._2 = i[2];
				*pI++ = idx;
				_numTriangles++;				// �������� �ﰢ�� ���� ����
			}

			b[2] = f.isIn(_heightMap[i[2]]);	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
			b[1] = f.isIn(_heightMap[i[1]]);	// ������� ������ Frustum�ȿ� �ִ°�?
			b[3] = f.isIn(_heightMap[i[3]]);	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
			if (b[2] | b[1] | b[3])				// ���߿� �ϳ��� frustum�ȿ� ������ �������Ѵ�.
			{
				idx._0 = i[2];
				idx._1 = i[1];
				idx._2 = i[3];
				*pI++ = idx;
				_numTriangles++;
			}
		}
	}
	_ib->Unlock();

	return true;
}

bool Terrain::initVertice(IDirect3DTexture9* texHeightMap)
{
	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;

	texHeightMap->GetLevelDesc(0, &ddsd);	// �ؽ�ó�� ����
	_numVertsCol = ddsd.Width;				// �ؽ�ó�� ����ũ��
	_numVertsRow = ddsd.Height;				// �ؽ�ó�� ����ũ��
	_heightMap = new Vector3[_numVertsCol * _numVertsRow];	// ���̸ʹ迭 ����

	if (FAILED(D3DDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height * sizeof(TERRAINVERTEX),
											 0, TERRAINVERTEX::FVF,
											 D3DPOOL_DEFAULT, &_vb, NULL)))
	{
		return false;
	}

	// �ؽ�ó �޸� ��!
	texHeightMap->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	VOID* pVertices;
	// �������� ��!
	if (FAILED(_vb->Lock(0, _numVertsCol*_numVertsRow * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
		return false;

	TERRAINVERTEX	v;
	TERRAINVERTEX*	pV = (TERRAINVERTEX*)pVertices;
	for (DWORD z = 0; z < _numVertsRow; z++)
	{
		for (DWORD x = 0; x < _numVertsCol; x++)
		{
			v.Position.x = (float)(x - _numVertsCol / 2.0f) * _cellSpacing;		// ������ x��ǥ(�޽ø� ������ ����)
			v.Position.z = -((float)(z - _numVertsRow / 2.0f) * _cellSpacing);	// ������ z��ǥ(�޽ø� ������ ����), z���� ����;����̹Ƿ� -�� ���Ѵ�.
			v.Position.y = ((float)(*((LPDWORD)d3drc.pBits + x + z*(d3drc.Pitch / 4)) & 0x000000ff)) * _heightScale;	/// DWORD�̹Ƿ� pitch/4
			v.Normal.x = v.Position.x;
			v.Normal.y = v.Position.y;
			v.Normal.z = v.Position.z;
			D3DXVec3Normalize(&v.Normal, &v.Normal);
			v.UV.x = (float)x / (_numVertsCol - 1);
			v.UV.y = (float)z / (_numVertsRow - 1);
			*pV++ = v;									// �������ۿ� ���� ����
			//_heightMap[z * _numVertsCol + x] = v.Position.y;	// ���̸ʿ� ���� ����
			_heightMap[z * _numVertsCol + x] = v.Position;	// ���̸ʿ� ���� ����
		}
	}

	_vb->Unlock();
	texHeightMap->UnlockRect(0);

	return true;
}

bool Terrain::initIndice()
{
	if (FAILED(D3DDevice->CreateIndexBuffer((_numVertsCol - 1)*(_numVertsRow - 1) * 2 * sizeof(INDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &_ib, NULL)))
	{
		return false;
	}

	_numTriangles = 0;
	return true;
}