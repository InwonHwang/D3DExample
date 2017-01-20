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
	ResourceHandle GetResourceHandle(const String& name);								// 리소스 핸들 얻기	

	sp<ResourceItem> GetEmptyResource();												// 레퍼런스 카운트가 0인 리소스.
	
	void RegisterResource(ResourceHandle handle, const sp<ResourceItem> resource);		// 리소스 등록.
	sp<ResourceItem> UnregisterResource(ResourceHandle handle);							// 리소스 등록 해제	

	ResourceHandle GenerateResourceHandle() const;										// 새로 생생되는 리소스의 핸들을 생성.


protected:
	uint				_resourceCount;	// 리소스 개수.
	ResourceNamesMap	_names;
	ResourceItemsMap	_items;
	ResourcePoolHandle	_handle;
};

inline ResourcePoolHandle ResourcePoolImpl::GetHandle() const
{
	return _handle;
}

