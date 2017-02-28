#include "ResourceTable.h"



ResourceTable::ResourceTable(ResourcePoolID handle)
	: _handle(handle)
{
}


ResourceTable::~ResourceTable()
{
}

String ResourceTable::GetName(ResourceID handle)
{
	return _nameMap[handle];
}

void ResourceTable::SetName(ResourceID handle, const String& name)
{
	_nameMap[handle] = name;	
}

sp<ResourceItem> ResourceTable::GetResource(ResourceID handle)
{
	auto it = _resourceMap.find(handle);

	if (it != _resourceMap.end())
		return it->second;

	return nullptr;
}

sp<ResourceItem> ResourceTable::GetResource(const String& name)
{
	ResourceID handle = GetResourceHandle(name);
	auto it = _resourceMap.find(handle);

	if (it != _resourceMap.end())
		return it->second;

	return nullptr;
}

void ResourceTable::Clear()
{	
	for (auto r : _resourceMap)
	{
		r.second->Destroy();
	}
	_nameMap.clear();
	_resourceMap.clear();
}

ResourceID ResourceTable::GetResourceHandle(const String& name)
{
	for (auto it = _nameMap.begin(); it != _nameMap.end(); ++it)
	{
		if (it->second.compare(name) == 0)
			return it->first;
	}

	return InvalidHandle;
}

sp<ResourceItem> ResourceTable::GetEmptyResource()
{
	for (auto it = _resourceMap.begin(); it != _resourceMap.end(); ++it)
	{
		// �����ϴ� ��ü�� ���� ���� �����Ͱ� �ε�� ���°� �ƴϰ� ��� �����͵� ���ٸ�
		if (it->second.use_count() == 1 &&
			it->second->IsLoaded() == false &&
			it->second->IsBackup() == false)
			return it->second;
	}

	return nullptr;
}

void ResourceTable::RegisterResource(ResourceID handle, const sp<ResourceItem> resource)
{
	auto it = _resourceMap.find(handle);

	//if (it == _resourceMap.end());
		// �����޼��� ��� �� ����
	
	
	_resourceMap[handle] = resource;
}

sp<ResourceItem> ResourceTable::UnregisterResource(ResourceID handle)
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

ResourceID ResourceTable::GenerateResourceHandle() const
{
	for (uint i = 0; i < _resourceCount; ++i)
	{
		auto it = _resourceMap.find(i);
		
		if (it == _resourceMap.end())
			return i;		
	}	

	return InvalidHandle;
}
