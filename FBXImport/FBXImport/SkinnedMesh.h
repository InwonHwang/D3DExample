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
	virtual void Draw() override;									// IDirect3DDevice9 필요

	// inherited from IMemento
	virtual void Load(FbxNode& fbxNode, void* pData) override;		// IDirect3DDevice9 필요
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
	std::vector<D3DXMATRIX>				_boneOffset;	// 오프셋 매트릭스
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
//	// 테스트용 나중에 SkinnedMeshRenderer 컴포넌트에서 대신 해줄것.
//	virtual void Draw() override;
//	virtual void ApplyMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matLocal);
//
//protected:
//	virtual void LoadVerticeAndIndice(FbxNode& fbxNode, void* pData) override;	// 버텍스 인덱스 로드
//
//	// class Mesh 에서 상속 받기
//	void LoadTexCoord(FbxMesh& fbxMesh, int index, int vertexId, FbxVector2& texCoord);		// UV 값 로드
//	void LoadNormal(FbxMesh& fbxMesh, int index, int vertexId, FbxVector4& normal);			// 노멀 값 로드
//	//void LoadBinormal(FbxMesh& fbxMesh, int index, FbxVector4 binormal);		// 바이노멀 값 로드
//	//void LoadTangent(FbxMesh& fbxMEsh, int index, FbxVector4 tangent);		// 탄젠트값 로드
//
//	// 가중치값 및 인덱스 로드
//	void LoadSkinInfo(FbxMesh& fbxMesh, std::vector<SKINNEDMESHVERTEX>& bufferVertice, std::vector<Frame *>& boneHierarchy);
//
//private:
//	// 스키닝 정보
//	std::vector<int>			_indexBones;	// 영향을 미치는 본의 인덱스
//	std::vector<D3DXMATRIX>		_boneOffset;	// 오프셋 매트릭스
//	std::vector<Frame *>*		_boneHierarchy;
//	
//};
//
