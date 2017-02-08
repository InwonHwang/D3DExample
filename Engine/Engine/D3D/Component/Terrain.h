#pragma once
#include "..\..\Core\Core.h"
#include "..\Resource\D3DResource.h"
#include "Component.h"

class Frustum;

class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	virtual void Destroy() override;

	virtual void Draw(IDirect3DDevice9& device);	// Renderer에서 상속 예정
	
	void SetTerrainData(const sp<TerrainData> terrainData);

	//Test 용
	void DrawFrustum(IDirect3DDevice9& device, sp<Frustum> frustum);

private:
	sp<TerrainData> _terrainData;
};

