#pragma once
#include "ResourcePoolImpl.h"

template<typename T>
class ResourcePool : public ResourcePoolImpl
{
	typedef boost::object_pool<T> MemoryPool;

public:
	ResourcePool(ResourcePoolHandle handle);
	~ResourcePool();

	//sp<T> Load();
	sp<T> Create();
	void Destroy(void* resource);
	
protected:
	MemoryPool	_allocator;	// 객체가 소멸될때 _allocator로부터 할당된 메모리 모두 자동 해제됨
};