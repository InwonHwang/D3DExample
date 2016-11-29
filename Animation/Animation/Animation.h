#pragma once
#include "IResourceItem.h"

// 1. operator overloading 해야함 (new, delete)
//   - animation 개수는 정해져있음.
//   - 사용자가 추가할 수 있음 -> 몇개를 추가할지는 모름
//   - 간단한 SRT animation은 Coroutine기능으로도 구현가능
//	 - 결과적으로 한 scene에 사용자가 몇개의 애니메이션을 추가적으로 생상하는지가 관건임.
//   - 몬스터의 움직임이나 장애물같은 경우가 여러 종류일때 애니메이션이 많이 생산됨.
//	 - 그러나 몬스터의 움직임은 FSM으로 Coroutine으로 구현될 가능성이 많음.
//   - 결과적으로 SingletonPool보다 ObjectPool을 사용하는 것으로 결론을 내림.
// 2. animationController 공유는 포인터 형태로 이루워져야 함.(Flyweight) ResourceManager에 의해 공유되어지거나 컴포넌트의 Get함수로 공유됨.
// 3. 추가적으로 public method 제공해야함.
//   - animationkey 등록하기, _animationData 자료형은 추후에 자료구조를 통해서 하는것이 좋을거 같음.(vector, list)
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
		DWORD				Index;			// track 순서
		DWORD				NumKeys;		// Animation Key 개수
		D3DXKEY_VECTOR3*	ScaleKeys;		// ScaleKey 정보
		D3DXKEY_QUATERNION* RotationKeys;	// RotationKey 정보
		D3DXKEY_VECTOR3*	PositionKeys;	// PositionKey 정보
		D3DXKEY_MATRIX*		MatrixKeys;		// MatrixKey 정보

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
	ID3DXKeyframedAnimationSet* _animationSet;	// ID3DXKeyframedAnimation 포인터
	ANIMATIONDATA* _animationData;				// AnimationKey정보가 있는 구조체
	DWORD _numAnimation;								// AnimationSet안에 있는 Animation(Bone) 개수

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

