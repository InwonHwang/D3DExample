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

	D3DXMATRIX GetLocalMatrix() const;
	D3DXMATRIX GetWorldMatrix() const;
	D3DXMATRIX GetWorldMatrixParent() const;

	const char* GetName() const;

	// Debug용
	void Draw(IDirect3DDevice9& device);

private:
	bool		_isBone;					// 현재 프레임이 본역할을 하는지 아닌지
	std::string _name;
	D3DXMATRIX	_matWorldParent;			// 부모 매트릭스, 월드 매트릭스 = 로컬 매트릭스 * 부모매트릭스
	D3DXMATRIX	_matLocal;					// 로컬 매트릭스
};

inline D3DXMATRIX Frame::GetLocalMatrix() const
{
	return _matLocal;
}

inline D3DXMATRIX Frame::GetWorldMatrix() const
{
	return _matLocal * _matWorldParent;
}

inline D3DXMATRIX Frame::GetWorldMatrixParent() const
{
	return _matWorldParent;
}


inline const char* Frame::GetName() const
{
	return _name.c_str();
}