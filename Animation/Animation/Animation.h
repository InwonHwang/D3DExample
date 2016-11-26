#pragma once
#include "IResourceItem.h"

typedef struct tagD3DXKeyMatrix
{
	FLOAT Time;
	D3DXMATRIX Value;
}D3DXKEY_MATRIX, *LPD3DXKEY_MATRIX;

typedef struct tagAnimationData
{
	char*				Name;
	DWORD				Index;			// track ����
	DWORD				NumKeys;	
	D3DXKEY_VECTOR3*	ScaleKeys;
	D3DXKEY_QUATERNION* RotationKeys;
	D3DXKEY_VECTOR3*	PositionKeys;
	D3DXKEY_MATRIX*		MatrixKeys;

	tagAnimationData() : Name(nullptr), ScaleKeys(nullptr), RotationKeys(nullptr), PositionKeys(nullptr), MatrixKeys(nullptr) {}
	~tagAnimationData()
	{
		SAFE_DELETE_ARRAY(Name);
		SAFE_DELETE_ARRAY(ScaleKeys);
		SAFE_DELETE_ARRAY(RotationKeys);
		SAFE_DELETE_ARRAY(PositionKeys);
		SAFE_DELETE_ARRAY(MatrixKeys);
	}
}ANIMATIONDATA, *LPANIMATIONDATA;

class Animation final : public IResourceItem
{
private:
	ID3DXKeyframedAnimationSet* _animationSet;	// ID3DXKeyframedAnimation ������
	ANIMATIONDATA* _animationData;				// AnimationKey������ �ִ� ����ü
	DWORD _numAnimation;						// AnimationSet�ȿ� �ִ� Animation ����

private:
	void loadAnimationKey(LPD3DXFILEDATA, void*);
	void loadAnimationName(LPD3DXFILEDATA, void*);

public:	
	virtual void save() override;
	virtual void load(LPD3DXFILEDATA) override;
	virtual void loadChild(LPD3DXFILEDATA, void*) override;
	virtual void load(TCHAR *) override;

public:
	void load(char *animName, float speed, bool loop);
	ID3DXKeyframedAnimationSet* GetAnimationSet() { return _animationSet; }

public:
	Animation();
	~Animation();
};

