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
	Pool	_allocator;	// 객체가 소멸될때 _allocator로부터 할당된 메모리 모두 자동 해제됨
};

template<typename T>
sp<T> ResourcePool<T>::Create()
{
	// 내부 데이터가 비어있는 리소스를 찾음.
	sp<T> empty = boost::static_pointer_cast<T>(GetEmptyResource());

	// 내부 데이터가 비어있는 리소스가 있으면 해당 리소스 리턴
	if (empty)
	{
		return empty;
	}
	// 없으면 새로 만들어서 리턴
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