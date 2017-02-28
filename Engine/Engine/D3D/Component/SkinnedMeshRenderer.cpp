#include "SkinnedMeshRenderer.h"


SkinnedMeshRenderer::SkinnedMeshRenderer()
	: _mesh(nullptr)
{
	_materials.reset(new MaterialVec);
	_matrix.reset(new BoneMatrixVec(64));	
	temp = new D3DXMATRIX[64];
}


SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	delete[] temp;
}

void SkinnedMeshRenderer::Destroy()
{
	_materials->clear();
	_materials.reset();
	_matrix->clear();
	_matrix.reset();
	_mesh.reset();
}

void SkinnedMeshRenderer::Draw(IDirect3DDevice9& device)
{
	if (!_mesh) return;

	//for (uint i = 0; i < 64; ++i)
	//{
	//	BOXVERTEX vtx[8];
	//	vtx[0] = BOXVERTEX(-2, 2, 2, 0xffff0000);		/// v0
	//	vtx[1] = BOXVERTEX(2, 2, 2, 0xffff0000);		/// v1
	//	vtx[2] = BOXVERTEX(2, 2, -2, 0xffff0000);		/// v2
	//	vtx[3] = BOXVERTEX(-2, 2, -2, 0xffff0000);		/// v3
	//	vtx[4] = BOXVERTEX(-2, -2, 2, 0xffff0000);		/// v4
	//	vtx[5] = BOXVERTEX(2, -2, 2, 0xffff0000);		/// v5
	//	vtx[6] = BOXVERTEX(2, -2, -2, 0xffff0000);		/// v6
	//	vtx[7] = BOXVERTEX(-2, -2, -2, 0xffff0000);		/// v7

	//	INDEX idx[12] =
	//	{
	//		{ 0, 1, 2 } ,{ 0, 2, 3 } ,	/// À­¸é
	//		{ 4, 6, 5 } ,{ 4, 7, 6 } ,	/// ¾Æ·§¸é
	//		{ 0, 3, 7 } ,{ 0, 7, 4 } ,	/// ¿Þ¸é
	//		{ 1, 5, 6 } ,{ 1, 6, 2 } ,	/// ¿À¸¥¸é
	//		{ 3, 2, 6 } ,{ 3, 6, 7 } ,	/// ¾Õ¸é
	//		{ 0, 4, 5 } ,{ 0, 5, 1 } 	/// µÞ¸é
	//	};

	//	device.SetTransform(D3DTS_WORLD, &_matrix->data()[i]);
	//	device.SetFVF(BOXVERTEX::FVF);
	//	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, idx, D3DFMT_INDEX32, vtx, sizeof BOXVERTEX);
	//}

	IDirect3DVertexDeclaration9* vd = _mesh->GetVertexBuffer()->GetD3DVertexDeclaration();
	IDirect3DVertexBuffer9* vb = _mesh->GetVertexBuffer()->GetD3DVertexBuffer();
	IDirect3DIndexBuffer9* ib = _mesh->GetIndexBuffer()->GetD3DIndexBuffer();
	int vertexCount = _mesh->GetVertexCount();
	int primitiveCount = _mesh->GetPrimitiveCount();

	
	//device.SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	//device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	device.SetStreamSource(0, vb, 0, sizeof(SKINNEDMESHVERTEX));	
	//device.SetVertexDeclaration(vd);
	device.SetFVF(SKINNEDMESHVERTEX::FVF);
	device.SetIndices(ib);

	//device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	//device.SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);

	uint materialCount = _materials->size();

	for (uint i = 0; i < materialCount; ++i)
	{
		sp<SurfaceMaterial> material = _materials->data()[i];		

		material->SetMatrixArray(_T("gMatrixPalette"), _matrix->data(), _matrix->size());

		uint numPass = 0;
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

void SkinnedMeshRenderer::ApplyMatrix(sp<Animation> animation, int frame)
{
	D3DXMATRIX m;
	for (uint i = 0; i < animation->GetAnimCurveVec()->size(); ++i)
	{		
		animation->GetAnimCurveVec()->data()[i]->GetAnimatedMatrix(frame, m);
		temp[i] = m;
		_matrix->data()[i] = m;
	}
}

void SkinnedMeshRenderer::Test(IDirect3DDevice9& device, sp<Animation> animation, int frame)
{
	//sp<std::vector<int>> boneIndex = _mesh->GetBoneIndex();

	//int vertexCount = _mesh->GetVertexCount();
	//int primitiveCount = _mesh->GetPrimitiveCount();
	//void* pIndex = _mesh->pIndex;
	//SKINNEDMESHVERTEX* pVertice = _mesh->pSkinnedMeshVertex;
	//TEMPVERTEX* pVerticeDraw = _mesh->pVertexDraw;
	//D3DXMATRIX m;
	//Vector3	   v;

	//D3DXMatrixIdentity(&m);
	//int index = 0;

	//for (int i = 0; i < vertexCount; ++i)
	//{
	//	pVerticeDraw[i].position = Vector3(0.0f, 0.0f, 0.0f);
	//	for (int j = 0; j < 4; ++j)
	//	{
	//		index = pVertice[i].index[j];
	//		D3DXMATRIX temp;
	//		m = FbxDXUtil::ToDXMatrix(_boneDataVec[index]->animVec[(frame - (frame % 60)) / 60]->globalTransform);
	//		//animation->GetAnimCurveVec()->data()[index]->GetAnimatedMatrix(frame, m);

	//		D3DXVec3TransformCoord(&v, &pVertice[i].position, &m);	// V_world(n) = V_local(n) * TM_bone(idx)
	//		pVerticeDraw[i].position += (v * pVertice[i].weight[j]); // V_world = V_world(0)+ ... +V_world(3)			
	//	}
	//}
	//D3DXMATRIX identity;
	//D3DXMatrixIdentity(&identity);
	//device.SetTransform(D3DTS_WORLD, &identity);

	//device.SetFVF(TEMPVERTEX::FVF);
	//device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vertexCount, primitiveCount, pIndex, D3DFMT_INDEX32, pVerticeDraw, sizeof(TEMPVERTEX));
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
