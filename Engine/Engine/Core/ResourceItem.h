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
		eLoaded,		// true �ε��. false �ε���� ����
		eBackup			// true ���޸𸮿� ��� ������ ����. false ��� ������ ����.
	};

public:
	ResourceItem(ResourceHandle handle, ResourcePoolImpl* pool);
	virtual ~ResourceItem();

	ResourceHandle GetHandle() const;
	String GetName() const;
	void SetName(const String& name) const;

	bool IsLoaded() const;		// ���� �����Ͱ� �ε��.
	bool IsBackup() const;		// �޸𸮸� ���� ����س�����

protected:
	void Load();				// eLoaded���� true�� ����. ���ε����Ͱ� �ε��.
	void Unload();				// eLoaded���� false�� ����. ���ε����Ͱ�	Release��
	void Copy();				// eBackup���� true�� ����. ��������� ������.
	void Clear();				// eBackup���� false�� ����. ��������Ͱ� ������.

protected:
	flag32			  _flagState;		// ���ҽ� ���� ����.
	ResourceHandle	  _handle;			// �ڵ鰪�� ���ؼ� �ٸ������� ���� �� ����.	
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