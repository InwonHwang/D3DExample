#include "Frame.h"
#include "Description.h"

Frame::Frame()
{
	D3DXMatrixIdentity(&_matLocal);
	D3DXMatrixIdentity(&_matWorldParent);
}


Frame::~Frame()
{
	_name.clear();
}

void Frame::SetLocalMatrix(D3DXMATRIX& m)
{
	_matLocal = m;
}

void Frame::SetName(const char* name)
{
	_name = name;
}

void Frame::Load(FbxNode& fbxNode, void* pData)
{
	_name = fbxNode.GetName();

	_matLocal = FbxDxUtil::ToDX(fbxNode.EvaluateGlobalTransform());
	
	FbxAMatrix matWorld;
	matWorld.SetIdentity();

	FbxNode* parent = fbxNode.GetParent();
	if (parent)	
		matWorld = parent->EvaluateGlobalTransform();
	
	
	_matWorldParent = FbxDxUtil::ToDX(matWorld);
}

void Frame::Save(FbxNode& fbxNode)
{

}

void Frame::Draw(IDirect3DDevice9& device)
{
	BOXVERTEX vtx[8];
	vtx[0] = BOXVERTEX(-5, 5, 5, 0xffff0000);		/// v0
	vtx[1] = BOXVERTEX(5, 5, 5, 0xffff0000);		/// v1
	vtx[2] = BOXVERTEX(5, 5, -5, 0xffff0000);		/// v2
	vtx[3] = BOXVERTEX(-5, 5, -5, 0xffff0000);		/// v3
	vtx[4] = BOXVERTEX(-5, -5, 5, 0xffff0000);		/// v4
	vtx[5] = BOXVERTEX(5, -5, 5, 0xffff0000);		/// v5
	vtx[6] = BOXVERTEX(5, -5, -5, 0xffff0000);		/// v6
	vtx[7] = BOXVERTEX(-5, -5, -5, 0xffff0000);		/// v7

	INDEX idx[12] =
	{
		{0, 1, 2} , {0, 2, 3} ,	/// À­¸é
		{4, 6, 5} , {4, 7, 6} ,	/// ¾Æ·§¸é
		{0, 3, 7} , {0, 7, 4} ,	/// ¿Þ¸é
		{1, 5, 6} , {1, 6, 2} ,	/// ¿À¸¥¸é
		{3, 2, 6} , {3, 6, 7} ,	/// ¾Õ¸é
		{0, 4, 5} , {0, 5, 1} 	/// µÞ¸é
	};

	D3DXMATRIX m = _matLocal * _matWorldParent;
	
	device.MultiplyTransform(D3DTS_WORLD, &m);
	device.SetFVF(BOXVERTEX::FVF);
	device.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, idx, D3DFMT_INDEX32, vtx, sizeof BOXVERTEX);
}