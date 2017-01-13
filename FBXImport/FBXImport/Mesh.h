#pragma once
#include "Core.h"
#include "Description.h"

class VertexBuffer;
class IndexBuffer;

// http://www.gpgstudy.com/forum/viewtopic.php?p=126468
// FVF�� TANGENT, BINORMAL�� ��� VertexElement9* ����ü�� CreateVertexDeclaration�� ����ؾ���.
// Shader ����� �߰��� ���� �ʿ�.
// �߰��ؾ� �� ����.
// IDirect3DVertexDeclaration9 �߰��ϱ�.

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

	virtual void Draw() abstract;						// Device �ʿ�

	bool IsSkinned() const;

protected:
	virtual void LoadPosition(FbxMesh& mesh, int ctrlPointIndex, D3DXVECTOR3& position) abstract;
	virtual void LoadColor(FbxMesh&, int ctrlPointIndex, int vertexCount, D3DXVECTOR4& color) abstract;
	virtual void LoadUV(FbxMesh&, int ctrlPointIndex, int uvIndex, D3DXVECTOR2& uv) abstract;
	virtual void LoadNormal(FbxMesh&, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& normal) abstract;
	virtual void LoadTangent(FbxMesh&, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& tangent) abstract;

protected:
	IDirect3DDevice9*	_device;
	VertexBuffer*		_vb;
	IndexBuffer* 		_ib;
	ID3DXMesh*			_mesh;

	bool	_isSkinned;				// �ִϸ��̼ǵ� �޽����� �ƴ���;
	int		_vertexCount;			// vertex����
	int		_primitiveCount;		// triangle(index) ����

};

inline bool Mesh::IsSkinned() const
{
	return _isSkinned;
}