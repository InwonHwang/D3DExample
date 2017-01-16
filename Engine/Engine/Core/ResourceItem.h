#pragma once
#include "BitFlag.h"

typedef unsigned int ResourceHandle;

class ResourceItem
{
public:
	enum ResourceItemState
	{
		eCreated,		// true ���� �����Ͱ� �޸𸮿� �Ҵ��.(������ ������ �ε���� ����.)
		eLoaded,		// true �ε��. false �ε���� ����
		eBackup			// true ���޸𸮿� ��� ������ ����. false ��� ������ ����.
	};

public:
	ResourceItem();
	virtual ~ResourceItem();

	bool IsCreated() const;		// ���� �����Ͱ� ���� �Ҵ��.
	bool IsLoaded() const;		// ���� �����Ͱ� �ε��.
	bool IsBackup() const;		// �޸𸮸� ���� ����س�����

	ResourceHandle GetHandle() const;

protected:
	void Create();				// eCreated flag�� true�� ����.
	void Destroy();				// eCreated flag�� false�� ����.
	void Load();				// eLoaded���� true�� ����.
	void Unload();				// eLoaded���� false�� ����.
	void Copy();				// eBackup���� true�� ����.
	void Clear();				// eBackup���� false�� ����.

protected:
	flag32			_flagState;			// ���ҽ� ���� ����.
	ResourceHandle	_handle;			// �ڵ鰪�� ���ؼ� �ٸ������� ���� �� ����.
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