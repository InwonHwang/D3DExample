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
	ResourceID GetResourceHandle(const String& name);								// ���ҽ� �ڵ� ���	

	sp<ResourceItem> GetEmptyResource();											// ���۷��� ī��Ʈ�� 0�� ���ҽ�.
	
	void RegisterResource(ResourceID handle, const sp<ResourceItem> resource);		// ���ҽ� ���.
	sp<ResourceItem> UnregisterResource(ResourceID handle);							// ���ҽ� ��� ����	

	ResourceID GenerateResourceHandle() const;										// ���� �����Ǵ� ���ҽ��� �ڵ��� ����.


protected:
	uint				_resourceCount;	// ���ҽ� ����.
	ResourceNamesMap	_nameMap;
	ResourceItemsMap	_resourceMap;
	ResourcePoolID	_handle;
};

inline ResourcePoolID ResourceTable::GetHandle() const
{
	return _handle;
}

