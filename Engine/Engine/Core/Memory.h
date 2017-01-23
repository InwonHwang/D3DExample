#pragma once

#include "../ThirdParty/ThirdParty.h"

// Memory Ŭ����
// ���� �Ҵ�ǰ� �����Ǵ� �����鿡 ���ؼ��� ����ϱ� ����.

template <typename T>
class Memory abstract
{	
	using SingletonPool = boost::singleton_pool<T, sizeof(T), boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>;
public:
	static T* Alloc();
	static void Free(void * p);

	static T* OrderedAlloc(size_t size);
	static void OrderedFree(void * p);

	static void Release() {}
	static void Clear() {}	//�Ҵ�Ǿ��� ��� �޸� Ŭ����
};

template <typename T>
inline T* Memory<T>::Alloc()
{
	T* ret = static_cast<T *>(SingletonPool::malloc());
	new (ret) T();
	return ret;
}

template <typename T>
inline void Memory<T>::Free(void * p)
{
	static_cast<T *>(p)->~T();
	SingletonPool::free(p);
}

template <typename T>
inline T* Memory<T>::OrderedAlloc(size_t size)
{
	T* ret = static_cast<T *>(SingletonPool::ordered_malloc(size));
	new (ret) T();
	return ret;
}

template <typename T>
inline void Memory<T>::OrderedFree(void * p)
{
	static_cast<T *>(p)->~T();
	SingletonPool::ordered_free(p);
}

#define ALLOC() Memory::Alloc()
#define FREE(p) Memory::Free(p)
//#define SAFE_RELEASE() Memory::Release()
#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }