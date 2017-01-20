#pragma once
#include "..\ThirdParty\ThirdParty.h"
#include "ResourceItem.h"
#include "DataStructure.h"
#include "DataTypes.h"
#include "Debug.h"

typedef unsigned int ResourcePoolHandle;

class ResourcePoolImpl
{
	typedef	std::map<ResourceHandle, sp<ResourceItem>> ResourceItemsMap;
	typedef std::map<ResourceHandle, String>		 ResourceNamesMap;
	
public:
	enum {
		InvalidHandle = 0xffffffff
	};

public:
	ResourcePoolImpl(ResourcePoolHandle handle);
	virtual ~ResourcePoolImpl();

	String	GetName(ResourceHandle handle);
	void	SetName(ResourceHandle handle, const String& name);

	sp<ResourceItem> GetResource(ResourceHandle handle);
	sp<ResourceItem> GetResource(const String& name);

	ResourcePoolHandle GetHandle() const;	
	void	Clear();

protected:
	ResourceHandle GetResourceHandle(const String& name);								// ���ҽ� �ڵ� ���	

	sp<ResourceItem> GetEmptyResource();												// ���۷��� ī��Ʈ�� 0�� ���ҽ�.
	
	void RegisterResource(ResourceHandle handle, const sp<ResourceItem> resource);		// ���ҽ� ���.
	sp<ResourceItem> UnregisterResource(ResourceHandle handle);							// ���ҽ� ��� ����	

	ResourceHandle GenerateResourceHandle() const;										// ���� �����Ǵ� ���ҽ��� �ڵ��� ����.


protected:
	uint				_resourceCount;	// ���ҽ� ����.
	ResourceNamesMap	_names;
	ResourceItemsMap	_items;
	ResourcePoolHandle	_handle;
};

inline ResourcePoolHandle ResourcePoolImpl::GetHandle() const
{
	return _handle;
}

