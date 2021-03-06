#include "Terrain.h"
#include "../Device.h"
#include "../Frustum.h"
#include "../QuadTree.h"
#include <fstream>

Terrain::Terrain(UINT cellSpacing, float heightScale)
	: _cellSpacing(cellSpacing),
	_heightScale(heightScale)
{
}

Terrain::~Terrain()
{
	
}

void Terrain::create(IDirect3DDevice9* device, IDirect3DTexture9* texHeightMap)
{
	initVertice(device, texHeightMap);
	initIndice(device);;
	_quadTree = new QuadTree(_numVertsCol, _numVertsRow);
	_quadTree->build(_heightMap);
}

void Terrain::destroy()
{
	SAFE_DELETE_ARRAY(_heightMap);
	SAFE_DELETE(_quadTree);	
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}

void Terrain::draw(IDirect3DDevice9* device, Frustum& f)
{
#ifdef _USE_INDEX16
	LPWORD pI;
#else
	LPDWORD pI;
#endif
	
	if (SUCCEEDED(_ib->Lock(0, (_numVertsCol - 1)*(_numVertsRow - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
	{
		_numTriangles = _quadTree->generateIndex(pI, _heightMap, f, 0.025f);
		_ib->Unlock();
	}

	device->SetStreamSource(0, _vb, 0, sizeof(TERRAINVERTEX));
	device->SetFVF(TERRAINVERTEX::FVF);
	device->SetIndices(_ib);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _numVertsCol*_numVertsRow, 0, _numTriangles);
}

bool Terrain::initVertice(IDirect3DDevice9* device, IDirect3DTexture9* texHeightMap)
{
	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;

	texHeightMap->GetLevelDesc(0, &ddsd);	// 텍스처의 정보
	_numVertsCol = ddsd.Width + 1;				// 텍스처의 가로크기
	_numVertsRow = ddsd.Height + 1;				// 텍스처의 세로크기
	_heightMap = new Vector3[_numVertsCol * _numVertsRow];	// 높이맵배열 생성

	if (FAILED(device->CreateVertexBuffer(_numVertsCol * _numVertsRow * sizeof(TERRAINVERTEX),
											 0, TERRAINVERTEX::FVF,
											 D3DPOOL_DEFAULT, &_vb, NULL)))
	{
		return false;
	}

	// 텍스처 메모리 락!
	texHeightMap->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	VOID* pVertices;

	if (FAILED(_vb->Lock(0, _numVertsCol*_numVertsRow * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
		return false;

	TERRAINVERTEX	v;
	TERRAINVERTEX*	pV = (TERRAINVERTEX*)pVertices;
	for (int z = 0; z < _numVertsRow; z++)
	{
		for (int x = 0; x < _numVertsCol; x++)
		{
			v.Position.x = (float)(x - _numVertsCol / 2.0f) * _cellSpacing;		// 정점의 x좌표(메시를 원점에 정렬)
			v.Position.z = -((float)(z - _numVertsRow / 2.0f)) * _cellSpacing;	// 정점의 z좌표(메시를 원점에 정렬), z축이 모니터안쪽이므로 -를 곱한다.
			v.Position.y = ((float)(*((LPDWORD)d3drc.pBits + x + z*(d3drc.Pitch / 4)) & 0x000000ff)) * _heightScale;
			D3DXVec3Normalize(&v.Normal, &v.Position);
			v.UV.x = (float)x / (_numVertsCol - 1);
			v.UV.y = (float)z / (_numVertsRow - 1);			
			_heightMap[x + z * _numVertsCol] = v.Position;	// 높이맵에 정점 저장			

			*pV++ = v;
		}
	}

	_vb->Unlock();
	texHeightMap->UnlockRect(0);

	return true;
}

bool Terrain::initIndice(IDirect3DDevice9* device)
{
#ifdef _USE_INDEX16
	if (FAILED(device->CreateIndexBuffer((_numVertsCol - 1)*(_numVertsRow - 1) * 2 * sizeof(INDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &_ib, NULL)))
		return false;
#else
	if (FAILED(device->CreateIndexBuffer((_numVertsCol - 1)*(_numVertsRow - 1) * 2 * sizeof(INDEX), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &_ib, NULL)))
		return false;
#endif

	_numTriangles = 0;
	return true;
}