#include "ResourcePool.h"



ResourcePool::ResourcePool()
{
}


ResourcePool::~ResourcePool()
{
}

String ResourcePool::GetName(ResourceHandle handle)
{
	return _names[handle];
}

String ResourcePool::SetName(ResourceHandle handle, const String& name)
{
	_names[handle] = name;
}

ResourceItem* ResourcePool::FindResource(ResourceHandle handle)
{
	for (auto it = _items.begin(); it != _items.end(); ++it)
	{
		if (it->first == handle)
			return it->second;
	}

	return nullptr;
}

ResourceItem* ResourcePool::GetEmptyResource()
{
	for (auto it = _items.begin(); it != _items.end(); ++it)
	{
		if (it->second->IsCreated() == 0)
			return it->second;
	}

	return nullptr;
}

void ResourcePool::DestroyResource(ResourceHandle handle)
{
	ResourceItem* item = FindResource(handle);	
	
	delete item;
	
}
void ResourcePool::RegisterResource(ResourceHandle handle, ResourceItem& resource)
{
	_items[handle] = &resource;
}

ResourceItem* ResourcePool::UnregisterResource(ResourceHandle handle)
{
	_items.erase(handle);
}

ResourceHandle ResourcePool::GenerateResourceHandle() const
{
	for (int i = 0; i < _resourceCount; ++i)
	{
		if (!_items[i])
			return i;
	}
}
