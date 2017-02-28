#pragma once
#include "..\ThirdParty\ThirdParty.h"
#include "ResourceItem.h"
#include "DataStructure.h"
#include "DataTypes.h"
#include "Debug.h"
#include "TableImpl.h"

typedef Handle ResourcePoolID;

class ResourceTable// : public TableImpl
{
	typedef	std::map<ResourceID, sp<ResourceItem>> ResourceItemsMap;
	typedef std::map<ResourceID, String>		 ResourceNamesMap;
	
public:
	enum ResourceType {
		eEffect,
		eIndexBuffer,
		eTexture,
		eVertexBuffer,
		eAnimation,
		eAnimationController,
		eSkinnedMesh,
		eStaticMesh,
		eSurfaceMaterial,
		eTerrainData
	};
	enum {
		InvalidHandle = 0xffffffff
	};

public:
	ResourceTable(ResourcePoolID handle);
	virtual ~ResourceTable();

	String	GetName(ResourceID handle);
	void	SetName(ResourceID handle, const String& name);

	sp<ResourceItem> GetResource(ResourceID handle);
	sp<ResourceItem> GetResource(const String& name);

	ResourcePoolID GetHandle() const;	
	void	Clear();

protected:
	ResourceID GetResourceHandle(const String& name);								// 리소스 핸들 얻기	

	sp<ResourceItem> GetEmptyResource();											// 레퍼런스 카운트가 0인 리소스.
	
	void RegisterResource(ResourceID handle, const sp<ResourceItem> resource);		// 리소스 등록.
	sp<ResourceItem> UnregisterResource(ResourceID handle);							// 리소스 등록 해제	

	ResourceID GenerateResourceHandle() const;										// 새로 생생되는 리소스의 핸들을 생성.


protected:
	uint				_resourceCount;	// 리소스 개수.
	ResourceNamesMap	_nameMap;
	ResourceItemsMap	_resourceMap;
	ResourcePoolID	_handle;
};

inline ResourcePoolID ResourceTable::GetHandle() const
{
	return _handle;
}

