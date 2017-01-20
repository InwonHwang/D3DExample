#include "ResourcePool.h"
#include "..\D3D\Resource\Texture.h"

template<typename T>
ResourcePool<T>::ResourcePool(ResourcePoolHandle handle)
	: ResourcePoolImpl(handle)
{
}

template<typename T>
ResourcePool<T>::~ResourcePool()
{
}

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
void ResourcePool<T>::Destroy(void* resource)
{
	T* temp = static_cast<T*>(resource);
	_allocator.destroy(temp);
}