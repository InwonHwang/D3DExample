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
	MemoryPool	_allocator;	// ��ü�� �Ҹ�ɶ� _allocator�κ��� �Ҵ�� �޸� ��� �ڵ� ������
};