#include "MeshRenderer.h"



MeshRenderer::MeshRenderer()
	: _mesh(nullptr)
{
	_materials.reset(new MaterialVec);
}


MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Destroy()
{
	_materials->clear();
	_materials.reset();
	_mesh.reset();
}

void MeshRenderer::Draw(IDirect3DDevice9& device)
{
	if (!_mesh) return;

	IDirect3DVertexDeclaration9* vd = _mesh->GetVertexBuffer()->GetD3DVertexDeclaration();
	IDirect3DVertexBuffer9* vb = _mesh->GetVertexBuffer()->GetD3DVertexBuffer();
	IDirect3DIndexBuffer9* ib = _mesh->GetIndexBuffer()->GetD3DIndexBuffer();

	device.SetStreamSource(0, vb, 0, sizeof(STATICMESHVERTEX));
	device.SetVertexDeclaration(vd);
	device.SetIndices(ib);
	int vertexCount = _mesh->GetVertexCount();
	int primitiveCount = _mesh->GetPrimitiveCount();

	uint materialCount = _materials->size();

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
	}
}

void MeshRenderer::SetMesh(const sp<Mesh> mesh)
{
	_mesh = mesh;
}

void MeshRenderer::SetMaterialCount(uint count)
{
	_materials->resize(count);
}

void MeshRenderer::SetMaterial(uint index, const sp<SurfaceMaterial> material)
{
	assert(index < _materials->size());
	assert(material);

	_materials->data()[index] = material;
}
