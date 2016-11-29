#pragma once
#include "IResourceItem.h"

// 1. operator overloading �ؾ��� (new, delete)
//   - animation ������ ����������.
//   - ����ڰ� �߰��� �� ���� -> ��� �߰������� ��
//   - ������ SRT animation�� Coroutine������ε� ��������
//	 - ��������� �� scene�� ����ڰ� ��� �ִϸ��̼��� �߰������� �����ϴ����� ������.
//   - ������ �������̳� ��ֹ����� ��찡 ���� �����϶� �ִϸ��̼��� ���� �����.
//	 - �׷��� ������ �������� FSM���� Coroutine���� ������ ���ɼ��� ����.
//   - ��������� SingletonPool���� ObjectPool�� ����ϴ� ������ ����� ����.
// 2. animationController ������ ������ ���·� �̷������ ��.(Flyweight) ResourceManager�� ���� �����Ǿ����ų� ������Ʈ�� Get�Լ��� ������.
// 3. �߰������� public method �����ؾ���.
//   - animationkey ����ϱ�, _animationData �ڷ����� ���Ŀ� �ڷᱸ���� ���ؼ� �ϴ°��� ������ ����.(vector, list)
// 

class AnimationController;

class Animation final : public IResourceItem
{
	friend class AnimationController;

	typedef struct tagD3DXKeyMatrix
	{
		FLOAT Time;
		D3DXMATRIX Value;
	}D3DXKEY_MATRIX, *LPD3DXKEY_MATRIX;

	typedef struct tagAnimationData
	{
		char*				Name;			// BoneName
		DWORD				Index;			// track ����
		DWORD				NumKeys;		// Animation Key ����
		D3DXKEY_VECTOR3*	ScaleKeys;		// ScaleKey ����
		D3DXKEY_QUATERNION* RotationKeys;	// RotationKey ����
		D3DXKEY_VECTOR3*	PositionKeys;	// PositionKey ����
		D3DXKEY_MATRIX*		MatrixKeys;		// MatrixKey ����

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

private:
	ID3DXKeyframedAnimationSet* _animationSet;	// ID3DXKeyframedAnimation ������
	ANIMATIONDATA* _animationData;				// AnimationKey������ �ִ� ����ü
	DWORD _numAnimation;								// AnimationSet�ȿ� �ִ� Animation(Bone) ����

private:
	void loadAnimationKey(LPD3DXFILEDATA, void*);
	void loadAnimationName(LPD3DXFILEDATA, void*);

public:	
	virtual void save() override;
	virtual void load(LPD3DXFILEDATA) override;
	virtual void loadChild(LPD3DXFILEDATA, void*) override;
	virtual void load(TCHAR *) override;	

public:
	Animation();
	~Animation();

	void create(char *animName, float speed, bool loop);
	DWORD getNumBone() const { return _numAnimation; }
	const ID3DXKeyframedAnimationSet* GetAnimationSet() const { return _animationSet; }
};

