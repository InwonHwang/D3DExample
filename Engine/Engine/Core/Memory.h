#pragma once

#include "../ThirdParty/ThirdParty.h"

// Memory 클래스
// 자주 할당되고 해제되는 변수들에 대해서만 사용하길 권장.

template <typename T>
class Memory abstract
{	
	using SingletonPool = boost::singleton_pool<T, sizeof(T), boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>;
public:
	static void Clear(); //할당되었던 모든 메모리 클리어

	static T* Alloc();
	static void Free(void * p);

	static T* OrderedAlloc(size_t size);
	static void OrderedFree(void * p);
};

template <typename T>
inline void Memory<T>::Clear()
{
	SingletonPool::purge_memory();
}

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

template <typename T>
inline void SafeRelease(T* p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
inline void SafeDelete(T* p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

template <typename T>
inline void SafeDeleteArray(T* p)
{
	if (p)
	{
		delete[] p;
		p = nullptr;
	}
}