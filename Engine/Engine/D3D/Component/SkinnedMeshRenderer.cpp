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

	device.SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	device.SetStreamSource(0, vb, 0, sizeof(SKINNEDMESHVERTEX));
	device.SetVertexDeclaration(vd);
	device.SetIndices(ib);
	device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);

	device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	device.SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);

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
	sp<std::vector<int>> boneIndex = _mesh->GetBoneIndex();

	int index = 0;

	for (uint i = 0; i < boneIndex->size(); ++i)
	{
		index = boneIndex->data()[i];
		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		if (_boneDataVec[index]->pAnimation)
		{
			m = FbxDXUtil::ToDXMatrix(_boneDataVec[index]->pAnimation->pNext->pNext->pNext->pNext->pNext->globalTransform);
		}
		device.SetTransform(D3DTS_WORLDMATRIX(index), &m);
	}
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
