#pragma once
#include "BitFlag.h"
#include "DataTypes.h"

class ResourcePoolImpl;

typedef unsigned int ResourceHandle;

class ResourceItem
{
public:
	enum State
	{
		eLoaded,		// true 로드됨. false 로드되지 않음
		eBackup			// true 힙메모리에 백업 데이터 저장. false 백업 데이터 없음.
	};

public:
	ResourceItem(ResourceHandle handle, ResourcePoolImpl* pool);
	virtual ~ResourceItem();

	ResourceHandle GetHandle() const;
	String GetName() const;
	void SetName(const String& name) const;

	bool IsLoaded() const;		// 내부 데이터가 로드됨.
	bool IsBackup() const;		// 메모리를 힙에 백업해놓는지

protected:
	void Load();				// eLoaded값을 true로 세팅. 내부데이터가 로드됨.
	void Unload();				// eLoaded값을 false로 세팅. 내부데이터가	Release됨
	void Copy();				// eBackup값을 true로 세팅. 백업데이터 생성됨.
	void Clear();				// eBackup값을 false로 세팅. 백업데이터가 삭제됨.

protected:
	flag32			  _flagState;		// 리소스 상태 저장.
	ResourceHandle	  _handle;			// 핸들값을 통해서 다른정보를 얻을 수 있음.	
	ResourcePoolImpl* _pool;
};

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