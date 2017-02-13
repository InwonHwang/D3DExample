#include "Terrain.h"
#include "..\Frustum.h"
#include "..\QuadTree.h"

Terrain::Terrain()
{	
}


Terrain::~Terrain()
{
}

void Terrain::Destroy()
{
	_quadTree.reset();
	_terrainData.reset();
}

void Terrain::Draw(IDirect3DDevice9& device)	// Renderer에서 상속 예정
{
	if (!_terrainData) return;

	sp<IndexBuffer> ib = _terrainData->GetIndexBuffer();
	int width = _terrainData->GetWidth();
	int primitiveCount = 0;

	void* pData = nullptr;
	if (ib->Lock(0, (width - 1) * (width - 1) * 2 * sizeof(INDEX), &pData, 0))
	{
		INDEX* pIndice = static_cast<INDEX *>(pData);

		for (int z = 0; z < width - 1; ++z)
		{
			for (int x = 0; x < width - 1; ++x)
			{
				INDEX index;

				index._0 = x + z * width;
				index._1 = x + (z + 1) * width;
				index._2 = (x + 1) + z * width;

				*pIndice++ = index;
				primitiveCount++;

				index._0 = x + (z + 1) * width;
				index._1 = (x + 1) + (z + 1) * width;
				index._2 = (x + 1) + z * width;

				*pIndice++ = index;
				primitiveCount++;
			}
		}
		ib->Unlock();
	}

	IDirect3DVertexDeclaration9* d3dvd = _terrainData->GetVertexBuffer()->GetD3DVertexDeclaration();
	IDirect3DVertexBuffer9* d3dvb = _terrainData->GetVertexBuffer()->GetD3DVertexBuffer();
	IDirect3DIndexBuffer9* d3dib = _terrainData->GetIndexBuffer()->GetD3DIndexBuffer();

	device.SetStreamSource(0, d3dvb, 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(d3dvd);
	device.SetIndices(d3dib);
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _terrainData->GetVerticeCount(), 0, primitiveCount);
}

void Terrain::SetTerrainData(const sp<TerrainData> terrainData)
{
	_terrainData = terrainData;
	uint width = _terrainData->GetWidth();
	_quadTree.reset(new QuadTree(width, width));
	_quadTree->Build(_terrainData->GetHeightMap());
}

void Terrain::DrawFrustum(IDirect3DDevice9& device, sp<Frustum> frustum)
{
	if (!_terrainData) return;

	sp<std::vector<Vector3>> heightMapVec = _terrainData->GetHeightMap();
	sp<IndexBuffer> ib = _terrainData->GetIndexBuffer();
	int width = _terrainData->GetWidth();
	int primitiveCount = 0;

	int		idxHeightMap[4]; // 높이맵 인덱스
	BOOL	b[3];			 // 임시로 저장할 frustum culling결과값
	INDEX	index;

	void* pData = nullptr;
	if (ib->Lock(0, (width - 1) * (width - 1) * 2 * sizeof(INDEX), &pData, 0))
	{
		INDEX* pIndice = static_cast<INDEX *>(pData);

		for (int z = 0; z < width - 1; ++z)
		{
			for (int x = 0; x < width - 1; ++x)
			{
				idxHeightMap[0] = x + z * width;
				idxHeightMap[1] = (x + 1) + z * width;
				idxHeightMap[2] = x + (z + 1) * width;				
				idxHeightMap[3] = (x + 1) + (z + 1) * width;

				b[0] = frustum->IsIn(heightMapVec->data()[idxHeightMap[0]]);
				b[1] = frustum->IsIn(heightMapVec->data()[idxHeightMap[1]]);
				b[2] = frustum->IsIn(heightMapVec->data()[idxHeightMap[2]]);

				if (b[0] && b[1] && b[2])
				{
					index._0 = idxHeightMap[0];
					index._1 = idxHeightMap[2];
					index._2 = idxHeightMap[1];

					*pIndice++ = index;
					primitiveCount++;
				}

				b[0] = frustum->IsIn(heightMapVec->data()[idxHeightMap[1]]);
				b[1] = frustum->IsIn(heightMapVec->data()[idxHeightMap[2]]);
				b[2] = frustum->IsIn(heightMapVec->data()[idxHeightMap[3]]);

				if (b[0] && b[1] && b[2])
				{
					index._0 = idxHeightMap[2];
					index._1 = idxHeightMap[3];
					index._2 = idxHeightMap[1];					

					*pIndice++ = index;
					primitiveCount++;
				}
			}
		}
		ib->Unlock();
	}

	IDirect3DVertexDeclaration9* d3dvd = _terrainData->GetVertexBuffer()->GetD3DVertexDeclaration();
	IDirect3DVertexBuffer9* d3dvb = _terrainData->GetVertexBuffer()->GetD3DVertexBuffer();
	IDirect3DIndexBuffer9* d3dib = _terrainData->GetIndexBuffer()->GetD3DIndexBuffer();

	device.SetStreamSource(0, d3dvb, 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(d3dvd);
	device.SetIndices(d3dib);
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _terrainData->GetVerticeCount(), 0, primitiveCount);
}

void Terrain::DrawLOD(IDirect3DDevice9& device, sp<Frustum> frustum)
{
	if (!_terrainData) return;

	sp<IndexBuffer> indexBuffer = _terrainData->GetIndexBuffer();	
	uint width = _terrainData->GetWidth();
	uint primitiveCount = 0;

	INDEX* pI;
	

	if (SUCCEEDED(indexBuffer->Lock(0, (width - 1)*(width - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
	{
		primitiveCount = _quadTree->GenerateIndex(pI, _terrainData->GetHeightMap(), frustum, 0.025f);
		indexBuffer->Unlock();
	}

	IDirect3DVertexDeclaration9* d3dvd = _terrainData->GetVertexBuffer()->GetD3DVertexDeclaration();
	IDirect3DVertexBuffer9* d3dvb = _terrainData->GetVertexBuffer()->GetD3DVertexBuffer();
	IDirect3DIndexBuffer9* d3dib = _terrainData->GetIndexBuffer()->GetD3DIndexBuffer();

	device.SetStreamSource(0, d3dvb, 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(d3dvd);
	device.SetIndices(d3dib);
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _terrainData->GetVerticeCount(), 0, primitiveCount);
}
