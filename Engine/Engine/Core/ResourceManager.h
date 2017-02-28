#pragma once
#include "ResourcePool.h"
#include "ResourceItem.h"

class ResourceManager
{
	typedef std::map<ResourcePoolID, sp<ResourceTable>> ResourcePoolMap;	
public:
	ResourceManager();
	~ResourceManager();

	template<typename T>
	sp<T> Create();

	void Destroy();
	
private:
	template<typename T>
	sp<ResourcePool<T>> CreateResourcePool();					// 리소스풀 생성

	template<typename T>
	sp<ResourcePool<T>> FindResourcePool() const;				// 자료형으로 리소스 풀 찾기...

	ResourcePoolID GenerateResourcePoolHandle() const;		// 새로 생생되는 리소스풀 핸들을 생성.

private:
	ResourcePoolMap _poolMap;
};

template<typename T>
sp<T> ResourceManager::Create()
{
	// 해당 타입의 리소스 풀 찾기
	sp<ResourcePool<T>> pool = FindResourcePool<T>();

	// 없으면 만들기
	if (!pool)	
		pool = CreateResourcePool<T>();

	return pool->Create();
}

template<typename T>
sp<ResourcePool<T>> ResourceManager::CreateResourcePool()
{
	ResourcePoolID handle = GenerateResourcePoolHandle();	
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



