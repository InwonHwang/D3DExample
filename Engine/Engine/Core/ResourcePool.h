#pragma once
#include "ResourcePoolImpl.h"

template<typename T>
class ResourcePool : public ResourcePoolImpl
{
	typedef boost::object_pool<T> Pool;

public:
	ResourcePool(ResourcePoolHandle handle)	: ResourcePoolImpl(handle) {}
	~ResourcePool() {}

	//sp<T> Load();
	sp<T> Create();
	void Destroy(void* resource);
	
protected:
	Pool	_allocator;	// ��ü�� �Ҹ�ɶ� _allocator�κ��� �Ҵ�� �޸� ��� �ڵ� ������
};

template<typename T>
sp<T> ResourcePool<T>::Create()
{
	// ���� �����Ͱ� ����ִ� ���ҽ��� ã��.
	sp<T> empty = boost::static_pointer_cast<T>(GetEmptyResource());

	// ���� �����Ͱ� ����ִ� ���ҽ��� ������ �ش� ���ҽ� ����
	if (empty)
	{
		return empty;
	}
	// ������ ���� ���� ����
	else
	{
		_resourceCount++;
		ResourceHandle handle = GenerateResourceHandle();
		T* resource = _allocator.construct(handle, this);
		sp<T> newReousrce(resource, [](const void*) {});
		RegisterResource(handle, newReousrce);
		SetName(handle, _T(""));
		return newReousrce;
	}
}

template<typename T>
inline void ResourcePool<T>::Destroy(void* resource)
{
	T* temp = static_cast<T*>(resource);
	_allocator.destroy(temp);
}