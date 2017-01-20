#include "ResourceItem.h"
#include "ResourcePool.h"

ResourceItem::ResourceItem(ResourceHandle handle, ResourcePoolImpl* pool)
	: _handle(handle),
	_pool(pool)	
{
	_flagState.clear();
}

ResourceItem::~ResourceItem()
{
}

String ResourceItem::GetName() const
{
	return _pool->GetName(_handle);
}

void ResourceItem::SetName(const String& name) const
{
	_pool->SetName(_handle, name);
}
