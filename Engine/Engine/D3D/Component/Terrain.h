#pragma once
#include "..\..\Core\Core.h"
#include "..\Resource\D3DResource.h"
#include "Component.h"

class Frustum;
class QuadTree;

class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	virtual void Destroy() override;

	// friend private �Լ��� �ٲٱ�, Renderer���� ��� ����
	virtual void Draw(IDirect3DDevice9& device);
	void SetTerrainData(const sp<TerrainData> terrainData);

	//Test ��
	void DrawFrustum(IDirect3DDevice9& device, sp<Frustum> frustum);
	void DrawLOD(IDirect3DDevice9& device, sp<Frustum> frustum);

private:
	sp<TerrainData> _terrainData;
	sp<QuadTree>	_quadTree;
};

