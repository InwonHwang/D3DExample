#include "SkinnedMeshRenderer.h"


SkinnedMeshRenderer::SkinnedMeshRenderer()
	: _mesh(nullptr)
{
	_materials.reset(new MaterialVec);
}


SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::Destroy()
{
	_materials->clear();
	_materials.reset();
	_mesh.reset();
}

void SkinnedMeshRenderer::Draw(IDirect3DDevice9& device)
{
	if (!_mesh) return;

	IDirect3DVertexDeclaration9* vd = _mesh->GetVertexBuffer()->GetD3DVertexDeclaration();
	IDirect3DVertexBuffer9* vb = _mesh->GetVertexBuffer()->GetD3DVertexBuffer();
	IDirect3DIndexBuffer9* ib = _mesh->GetIndexBuffer()->GetD3DIndexBuffer();
	int vertexCount = _mesh->GetVertexCount();
	int primitiveCount = _mesh->GetPrimitiveCount();

	ApplyMatrix(device);

	/*device.SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	device.SetStreamSource(0, vb, 0, sizeof(SKINNEDMESHVERTEX));
	device.SetVertexDeclaration(vd);
	device.SetIndices(ib);
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);

	device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	device.SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);*/

	/*uint materialCount = _materials->size();

	for (uint i = 0; i < materialCount; ++i)
	{
		sp<SurfaceMaterial> material = _materials->data()[i];

		uint numPass;
		material->Begin(numPass);

		for (uint j = 0; j < numPass; ++j)
		{
			material->BeginPass(j);

			device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);

			material->EndPass();
		}

		material->End();
	}*/
}

void SkinnedMeshRenderer::ApplyMatrix(IDirect3DDevice9& device)
{
	/*sp<std::vector<int>> boneIndex = _mesh->GetBoneIndex();

	int index = 0;

	for (uint i = 0; i < boneIndex->size(); ++i)
	{
		index = boneIndex->data()[i];
		D3DXMATRIX m = FbxDXUtil::ToDXMatrix(_boneDataVec[index]->animVec[0]->globalTransform);		
		device.SetTransform(D3DTS_WORLDMATRIX(index), &(m));
	}*/

	sp<std::vector<int>> boneIndex = _mesh->GetBoneIndex();
		
	int vertexCount = _mesh->GetVertexCount();
	int primitiveCount = _mesh->GetPrimitiveCount();
	void* pIndex = _mesh->pIndex;
	SKINNEDMESHVERTEX* pVertice = _mesh->pSkinnedMeshVertex;
	TEMPVERTEX* pVerticeDraw = _mesh->pVertexDraw;
	D3DXMATRIX m;
	
	D3DXMatrixIdentity(&m);
	Vector3	   v;

	int index = 0;

	for (uint i = 0; i < vertexCount; ++i)
	{
		pVerticeDraw[i].position = Vector3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < 4; ++j)
		{
			index = pVertice[i].index[j];
			m = FbxDXUtil::ToDXMatrix(_boneDataVec[index]->animVec[10]->globalTransform);
			D3DXVec3TransformCoord(&v, &pVertice[i].position, &m);	// V_world(n) = V_local(n) * TM_bone(idx)
			pVerticeDraw[i].position += (v * pVertice[i].weight[j]); // V_world = V_world(0)+ ... +V_world(3)			
		}
	}
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	device.SetTransform(D3DTS_WORLD, &identity);

	device.SetFVF(TEMPVERTEX::FVF);
	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vertexCount, primitiveCount, pIndex, D3DFMT_INDEX32, pVerticeDraw, sizeof(TEMPVERTEX));
}

void SkinnedMeshRenderer::Test(IDirect3DDevice9& device, int frame)
{
	sp<std::vector<int>> boneIndex = _mesh->GetBoneIndex();

	int vertexCount = _mesh->GetVertexCount();
	int primitiveCount = _mesh->GetPrimitiveCount();
	void* pIndex = _mesh->pIndex;
	SKINNEDMESHVERTEX* pVertice = _mesh->pSkinnedMeshVertex;
	TEMPVERTEX* pVerticeDraw = _mesh->pVertexDraw;
	D3DXMATRIX m;
	Vector3	   v;

	D3DXMatrixIdentity(&m);
	

	int index = 0;

	for (uint i = 0; i < vertexCount; ++i)
	{
		pVerticeDraw[i].position = Vector3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < 4; ++j)
		{
			index = pVertice[i].index[j];
			m = FbxDXUtil::ToDXMatrix(_boneDataVec[index]->animVec[frame]->globalTransform);

			D3DXVec3TransformCoord(&v, &pVertice[i].position, &m);	// V_world(n) = V_local(n) * TM_bone(idx)
			pVerticeDraw[i].position += (v * pVertice[i].weight[j]); // V_world = V_world(0)+ ... +V_world(3)			
		}
	}
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	device.SetTransform(D3DTS_WORLD, &identity);

	device.SetFVF(TEMPVERTEX::FVF);
	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vertexCount, primitiveCount, pIndex, D3DFMT_INDEX32, pVerticeDraw, sizeof(TEMPVERTEX));
}


void SkinnedMeshRenderer::SetMesh(const sp<SkinnedMesh> mesh)
{
	_mesh = mesh;
}

void SkinnedMeshRenderer::SetMaterialCount(uint count)
{
	_materials->resize(count);
}

void SkinnedMeshRenderer::SetMaterial(uint index, const sp<SurfaceMaterial> material)
{
	assert(index < _materials->size());
	assert(material);

	_materials->data()[index] = material;
}
