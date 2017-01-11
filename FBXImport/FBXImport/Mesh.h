#pragma once
#include "Core.h"
#include "Description.h"

class VertexBuffer;
class IndexBuffer;

// http://www.gpgstudy.com/forum/viewtopic.php?p=126468
// FVF에 TANGENT, BINORMAL이 없어서 VertexElement9* 구조체와 CreateVertexDeclaration을 사용해야함.
// Shader 기능을 추가할 때도 필요.
// 추가해야 할 사항.
// IDirect3DVertexDeclaration9 추가하기.

class Mesh
{
public:
	Mesh(IDirect3DDevice9& device) :
		_device(&device),
		_vb(nullptr),
		_ib(nullptr),
		_isSkinned(false),
		_vertexCount(0),
		_primitiveCount(0)		
	{}
	virtual ~Mesh()	{}

	virtual void Draw() abstract;						// Device 필요

	bool IsSkinned() const;

protected:
	virtual void LoadVerticeAndIndice(FbxNode& fbxNode, void* pData) abstract;

protected:
	IDirect3DDevice9*	_device;
	VertexBuffer*		_vb;
	IndexBuffer* 		_ib;
	ID3DXMesh*			_mesh;

	bool	_isSkinned;				// 애니메이션된 메시인지 아닌지;
	int		_vertexCount;			// vertex개수
	int		_primitiveCount;		// triangle(index) 개수

};

inline bool Mesh::IsSkinned() const
{
	return _isSkinned;
}