#pragma once
#include "Mesh.h"
#include "IMemento.h"

class Frame;

class SkinnedMesh : public Mesh, public IMemento
{
public:
	SkinnedMesh(IDirect3DDevice9& device);
	virtual ~SkinnedMesh();

	void ApplyMatrix(D3DXMATRIX& matLocal, D3DXMATRIX& matWorld);

	// inherited from Mesh
	virtual void Draw() override;									// IDirect3DDevice9 �ʿ�

	// inherited from IMemento
	virtual void Load(FbxNode& fbxNode, void* pData) override;		// IDirect3DDevice9 �ʿ�
	virtual void Save(FbxNode& fbxNode) override;	

protected:
	void LoadSkinInfo(FbxNode& fbxNode, std::vector<SKINNEDMESHVERTEX>& bufferVertex);

	// inherited from Mesh
	virtual void LoadPosition(FbxMesh& mesh, int ctrlPointIndex, D3DXVECTOR3& position) override;
	virtual void LoadColor(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR4& color) override;
	virtual void LoadUV(FbxMesh& mesh, int ctrlPointIndex, int uvIndex, D3DXVECTOR2& uv) override;
	virtual void LoadNormal(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& normal) override;
	virtual void LoadTangent(FbxMesh& mesh, int ctrlPointIndex, int vertexCount, D3DXVECTOR3& tangent) override;
	

private:
	std::vector<int>					_indexBones;
	std::vector<D3DXMATRIX>				_boneOffset;	// ������ ��Ʈ����
	std::vector<Frame *>*				_bones;
};

//class Frame;
//
//class SkinnedMesh : public Mesh, public IMemento
//{
//public:
//	SkinnedMesh(IDirect3DDevice9& device);
//	~SkinnedMesh();
//	
//	virtual void Load(FbxNode& fbxNode, void* pData) override;
//	virtual void Save(FbxNode& fbxNode) override;
//
//	// �׽�Ʈ�� ���߿� SkinnedMeshRenderer ������Ʈ���� ��� ���ٰ�.
//	virtual void Draw() override;
//	virtual void ApplyMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matLocal);
//
//protected:
//	virtual void LoadVerticeAndIndice(FbxNode& fbxNode, void* pData) override;	// ���ؽ� �ε��� �ε�
//
//	// class Mesh ���� ��� �ޱ�
//	void LoadTexCoord(FbxMesh& fbxMesh, int index, int vertexId, FbxVector2& texCoord);		// UV �� �ε�
//	void LoadNormal(FbxMesh& fbxMesh, int index, int vertexId, FbxVector4& normal);			// ��� �� �ε�
//	//void LoadBinormal(FbxMesh& fbxMesh, int index, FbxVector4 binormal);		// ���̳�� �� �ε�
//	//void LoadTangent(FbxMesh& fbxMEsh, int index, FbxVector4 tangent);		// ź��Ʈ�� �ε�
//
//	// ����ġ�� �� �ε��� �ε�
//	void LoadSkinInfo(FbxMesh& fbxMesh, std::vector<SKINNEDMESHVERTEX>& bufferVertice, std::vector<Frame *>& boneHierarchy);
//
//private:
//	// ��Ű�� ����
//	std::vector<int>			_indexBones;	// ������ ��ġ�� ���� �ε���
//	std::vector<D3DXMATRIX>		_boneOffset;	// ������ ��Ʈ����
//	std::vector<Frame *>*		_boneHierarchy;
//	
//};
//
