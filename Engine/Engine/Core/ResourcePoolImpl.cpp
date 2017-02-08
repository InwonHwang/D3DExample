#include "ResourcePoolImpl.h"



ResourcePoolImpl::ResourcePoolImpl(ResourcePoolHandle handle)
	: _handle(handle)
{
}


ResourcePoolImpl::~ResourcePoolImpl()
{
}

String ResourcePoolImpl::GetName(ResourceHandle handle)
{
	return _nameMap[handle];
}

void ResourcePoolImpl::SetName(ResourceHandle handle, const String& name)
{
	_nameMap[handle] = name;	
}

sp<ResourceItem> ResourcePoolImpl::GetResource(ResourceHandle handle)
{
	auto it = _resourceMap.find(handle);

	if (it != _resourceMap.end())
		return it->second;

	return nullptr;
}

sp<ResourceItem> ResourcePoolImpl::GetResource(const String& name)
{
	ResourceHandle handle = GetResourceHandle(name);
	auto it = _resourceMap.find(handle);

	if (it != _resourceMap.end())
		return it->second;

	return nullptr;
}

void ResourcePoolImpl::Clear()
{	
	for (auto r : _resourceMap)
	{
		r.second->Destroy();
	}
	_nameMap.clear();
	_resourceMap.clear();
}

ResourceHandle ResourcePoolImpl::GetResourceHandle(const String& name)
{
	for (auto it = _nameMap.begin(); it != _nameMap.end(); ++it)
	{
		if (it->second.compare(name) == 0)
			return it->first;
	}

	return InvalidHandle;
}

sp<ResourceItem> ResourcePoolImpl::GetEmptyResource()
{
	for (auto it = _resourceMap.begin(); it != _resourceMap.end(); ++it)
	{
		// 참조하는 객체가 없고 내부 데이터가 로드된 상태가 아니고 백업 데이터도 없다면
		if (it->second.use_count() == 1 &&
			it->second->IsLoaded() == false &&
			it->second->IsBackup() == false)
			return it->second;
	}

	return nullptr;
}

void ResourcePoolImpl::RegisterResource(ResourceHandle handle, const sp<ResourceItem> resource)
{
	auto it = _resourceMap.find(handle);

	//if (it == _resourceMap.end());
		// 에러메세지 출력 후 리턴
	
	
	_resourceMap[handle] = resource;
}

sp<ResourceItem> ResourcePoolImpl::UnregisterResource(ResourceHandle handle)
{
	auto it = _resourceMap.find(handle);
	

	if (it != _resourceMap.end())
	{
		auto ret = it->second;
		_resourceMap.erase(it);
		return ret;
	}

	return nullptr;
}

ResourceHandle ResourcePoolImpl::GenerateResourceHandle() const
{
	for (uint i = 0; i < _resourceCount; ++i)
	{
		auto it = _resourceMap.find(i);
		
		if (it == _resourceMap.end())
			return i;		
	}	

	return InvalidHandle;
}
