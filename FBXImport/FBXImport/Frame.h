#pragma once
#include "Core.h"
#include "IMemento.h"

class Frame : public IMemento
{	

public:
	Frame();
	~Frame();
	
	virtual void Load(FbxNode& fbxNode, void* pData) override;
	virtual void Save(FbxNode& fbxNode) override;

	void SetLocalMatrix(D3DXMATRIX& m);
	void SetName(const char* name);

	D3DXMATRIX GetWorldMatrixParent() const;		// �θ���� ��Ʈ������ ���س��� ��.
	D3DXMATRIX GetLocalMatrix() const;
	D3DXMATRIX GetWorldMatrix() const;
	const char* GetName() const;

	// Debug��
	void Draw(IDirect3DDevice9& device);

private:
	std::string _name;
	D3DXMATRIX _matWorldParent;			// �θ� ��Ʈ����, ���� ��Ʈ���� = ���� ��Ʈ���� * �θ��Ʈ����
	D3DXMATRIX _matLocal;				// ���� ��Ʈ����
};

inline D3DXMATRIX Frame::GetWorldMatrixParent() const
{
	return _matWorldParent;
}

inline D3DXMATRIX Frame::GetLocalMatrix() const
{
	return _matLocal;
}

inline D3DXMATRIX Frame::GetWorldMatrix() const
{
	return _matLocal * _matWorldParent;
}

inline const char* Frame::GetName() const
{
	return _name.c_str();
}