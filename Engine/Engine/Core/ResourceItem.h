#pragma once
#include "BitFlag.h"

typedef unsigned int ResourceHandle;

class ResourceItem
{
public:
	enum ResourceItemState
	{
		eCreated,		// true 내부 데이터가 메모리에 할당됨.(데이터 내용은 로드되지 않음.)
		eLoaded,		// true 로드됨. false 로드되지 않음
		eBackup			// true 힙메모리에 백업 데이터 저장. false 백업 데이터 없음.
	};

public:
	ResourceItem();
	virtual ~ResourceItem();

	bool IsCreated() const;		// 내부 데이터가 힙에 할당됨.
	bool IsLoaded() const;		// 내부 데이터가 로드됨.
	bool IsBackup() const;		// 메모리를 힙에 백업해놓는지

	ResourceHandle GetHandle() const;

protected:
	void Create();				// eCreated flag를 true로 세팅.
	void Destroy();				// eCreated flag를 false로 세팅.
	void Load();				// eLoaded값을 true로 세팅.
	void Unload();				// eLoaded값을 false로 세팅.
	void Copy();				// eBackup값을 true로 세팅.
	void Clear();				// eBackup값을 false로 세팅.

protected:
	flag32			_flagState;			// 리소스 상태 저장.
	ResourceHandle	_handle;			// 핸들값을 통해서 다른정보를 얻을 수 있음.
};

inline bool ResourceItem::IsCreated() const
{
	return _flagState.check(eCreated);
}

inline bool ResourceItem::IsLoaded() const
{
	return _flagState.check(eLoaded);
}
inline bool ResourceItem::IsBackup() const
{
	return _flagState.check(eBackup);
}

inline ResourceHandle ResourceItem::GetHandle() const
{
	return _handle;
}

inline void ResourceItem::Create()
{
	_flagState.set(eCreated);
}

inline void ResourceItem::Destroy()
{
	_flagState.reset(eCreated);
}
inline void ResourceItem::Load()
{
	_flagState.set(eLoaded);
}
inline void ResourceItem::Unload()
{
	_flagState.reset(eLoaded);
}

inline void ResourceItem::Copy()
{
	_flagState.set(eBackup);
}

inline void ResourceItem::Clear()
{
	_flagState.reset(eBackup);
}