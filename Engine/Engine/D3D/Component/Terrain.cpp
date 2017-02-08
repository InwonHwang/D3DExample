#include "Terrain.h"
#include "..\Description\D3DDescription.h"
#include "..\Frustum.h"

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::Destroy()
{
	_terrainData.reset();
}

void Terrain::Draw(IDirect3DDevice9& device)	// Renderer에서 상속 예정
{
	if (!_terrainData) return;

	device.SetStreamSource(0, _terrainData->GetD3DVertexBuffer(), 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(_terrainData->GetD3DVertexDeclaration());
	device.SetIndices(_terrainData->GetD3DIndexBuffer());
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _terrainData->GetVerticeCount(), 0, _terrainData->GetPrimitiveCount());
}

void Terrain::SetTerrainData(const sp<TerrainData> terrainData)
{
	_terrainData = terrainData;
}

void Terrain::DrawFrustum(IDirect3DDevice9& device, sp<Frustum> frustum)
{
	if (!_terrainData) return;

	IDirect3DIndexBuffer9* indexBuffer = _terrainData->GetD3DIndexBuffer();
	uint width = _terrainData->GetWidth();

	BOOL		b[4];	// 임시로 저장할 frustum culling결과값
	INDEX		index;
	INDEX*		pI;

	uint primitiveCount = 0;;

	if (SUCCEEDED(indexBuffer->Lock(0, (width - 1)*(width - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
	{
		for (uint z = 0; z < width - 1; z++)
		{
			for (uint x = 0; x < width - 1; x++)
			{
				b[0] = frustum->IsIn(_terrainData->GetHeightData(x, z));	// 좌측상단 정점이 Frustum안에 있는가?
				b[1] = frustum->IsIn(_terrainData->GetHeightData(x + 1, z));	// 우측상단 정점이 Frustum안에 있는가?
				b[2] = frustum->IsIn(_terrainData->GetHeightData(x, z + 1));	// 좌측하단 정점이 Frustum안에 있는가?
				if (b[0] | b[1] | b[2])	// 셋중에 하나라도 frustum안에 있으면 렌더링한다.
				{
					index._0 = x + z * width;
					index._1 = x + (z + 1) * width;
					index._2 = (x + 1) + z * width;

					*pI++ = index;
					primitiveCount++;			// 렌더링할 삼각형 개수 증가
				}

				b[2] = frustum->IsIn(_terrainData->GetHeightData(x, z + 1));	// 좌측하단 정점이 Frustum안에 있는가?
				b[1] = frustum->IsIn(_terrainData->GetHeightData(x + 1, z));	// 우측상단 정점이 Frustum안에 있는가?
				b[3] = frustum->IsIn(_terrainData->GetHeightData(x + 1, z + 1));	// 우측하단 정점이 Frustum안에 있는가?
				if (b[2] | b[1] | b[3])	// 셋중에 하나라도 frustum안에 있으면 렌더링한다.
				{
					index._0 = x + (z + 1) * width;
					index._1 = (x + 1) + (z + 1) * width;
					index._2 = (x + 1) + z * width;

					*pI++ = index;
					primitiveCount++;
				}
			}
		}
		indexBuffer->Unlock();
	}

	device.SetStreamSource(0, _terrainData->GetD3DVertexBuffer(), 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(_terrainData->GetD3DVertexDeclaration());
	device.SetIndices(_terrainData->GetD3DIndexBuffer());
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _terrainData->GetVerticeCount(), 0, _terrainData->GetPrimitiveCount());
}
