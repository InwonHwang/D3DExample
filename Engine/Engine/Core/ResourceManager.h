#pragma once
#include "ResourcePool.h"
#include "ResourceItem.h"

class ResourceManager
{
	typedef std::map<ResourcePoolHandle, sp<ResourcePoolImpl>> ResourcePoolMap;	
public:
	ResourceManager();
	~ResourceManager();

	template<typename T>
	sp<T> Create();

	void Destroy();
	
private:
	template<typename T>
	sp<ResourcePool<T>> CreateResourcePool();					// ���ҽ�Ǯ ����

	template<typename T>
	sp<ResourcePool<T>> FindResourcePool() const;				// �ڷ������� ���ҽ� Ǯ ã��...

	ResourcePoolHandle GenerateResourcePoolHandle() const;		// ���� �����Ǵ� ���ҽ�Ǯ �ڵ��� ����.

private:
	ResourcePoolMap _poolMap;
};

template<typename T>
sp<T> ResourceManager::Create()
{
	// �ش� Ÿ���� ���ҽ� Ǯ ã��
	sp<ResourcePool<T>> pool = FindResourcePool<T>();

	// ������ �����
	if (!pool)	
		pool = CreateResourcePool<T>();

	return pool->Create();
}

template<typename T>
sp<ResourcePool<T>> ResourceManager::CreateResourcePool()
{
	ResourcePoolHandle handle = GenerateResourcePoolHandle();	
	sp<ResourcePool<T>> ret = sp<ResourcePool<T>>(new ResourcePool<T>(handle));
	_poolMap[handle] = ret;

	return ret;
}

template<typename T>
sp<ResourcePool<T>> ResourceManager::FindResourcePool() const
{
	for (auto it = _poolMap.begin(); it != _poolMap.end(); ++it)
	{
		sp<ResourcePool<T>> ret = boost::dynamic_pointer_cast<ResourcePool<T>>((*it).second);
		
		if (ret) return ret;
	}

	return nullptr;
}



