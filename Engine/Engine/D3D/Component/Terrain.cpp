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

void Terrain::Draw(IDirect3DDevice9& device)	// Renderer���� ��� ����
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

	BOOL		b[4];	// �ӽ÷� ������ frustum culling�����
	INDEX		index;
	INDEX*		pI;

	uint primitiveCount = 0;;

	if (SUCCEEDED(indexBuffer->Lock(0, (width - 1)*(width - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
	{
		for (uint z = 0; z < width - 1; z++)
		{
			for (uint x = 0; x < width - 1; x++)
			{
				b[0] = frustum->IsIn(_terrainData->GetHeightData(x, z));	// ������� ������ Frustum�ȿ� �ִ°�?
				b[1] = frustum->IsIn(_terrainData->GetHeightData(x + 1, z));	// ������� ������ Frustum�ȿ� �ִ°�?
				b[2] = frustum->IsIn(_terrainData->GetHeightData(x, z + 1));	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
				if (b[0] | b[1] | b[2])	// ���߿� �ϳ��� frustum�ȿ� ������ �������Ѵ�.
				{
					index._0 = x + z * width;
					index._1 = x + (z + 1) * width;
					index._2 = (x + 1) + z * width;

					*pI++ = index;
					primitiveCount++;			// �������� �ﰢ�� ���� ����
				}

				b[2] = frustum->IsIn(_terrainData->GetHeightData(x, z + 1));	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
				b[1] = frustum->IsIn(_terrainData->GetHeightData(x + 1, z));	// ������� ������ Frustum�ȿ� �ִ°�?
				b[3] = frustum->IsIn(_terrainData->GetHeightData(x + 1, z + 1));	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
				if (b[2] | b[1] | b[3])	// ���߿� �ϳ��� frustum�ȿ� ������ �������Ѵ�.
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
