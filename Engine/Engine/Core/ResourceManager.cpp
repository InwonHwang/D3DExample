#include "ResourceManager.h"
#include "..\D3D\ResourceBase\D3DResourceBase.h"


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Destroy()
{
	for (auto it = _poolMap.begin(); it != _poolMap.end(); ++it)
	{
		(*it).second->Clear();	// Ǯ�� �Ҵ�� ��� �޸� ����
	}
	_poolMap.clear();
}

ResourcePoolHandle ResourceManager::GenerateResourcePoolHandle() const
{	
	return _poolMap.size() + 1;
}
