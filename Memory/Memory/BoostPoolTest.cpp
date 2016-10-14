#include <iostream>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <windows.h>


#include <boost/shared_ptr.hpp>  
//#include <boost/enable_shared_from_this.hpp>  

#include "Timer.h"

using namespace std;

template<class T>
class PoolAlloc;

template<class T>
class ObjectPoolAlloc;

template<class T>
class SingletonPoolAlloc;

class Test;


template<class T>
class Alloc
{
public:
	static boost::shared_ptr<T> allocate()
	{
		cout << "test" << endl;
		return boost::shared_ptr<T>(boost::singleton_pool<T>::allocate());
	}	
};

template<class T>
class PoolAlloc
{
public:
	static void* operator new(size_t _size)
	{
		return pool.malloc();
	}
	static void operator delete(void* _p)
	{
		pool.free(_p);
	}
protected:
	static boost::pool<T> pool;
};
template<class T>
boost::pool<T> PoolAlloc<T>::pool(sizeof(T));

template<class T>
class ObjectPoolAlloc
{
public:
	static void* operator new(size_t _size)
	{
		return pool.malloc();
	}
	static void operator delete(void* _p)
	{
		pool.free((T * const)_p);
	}
protected:
	static boost::object_pool<T> pool;
};
template<class T>
boost::object_pool<T> ObjectPoolAlloc<T>::pool(sizeof(T));


//singleton_pool은 실제로 정의되어있음. 프로그램 시작부터 끝까지 실행됨, Thread safely.
template<class T>
class SingletonPoolAlloc
{
public:
	static void* operator new(size_t _size)
	{
		return boost::singleton_pool<T, sizeof(T), boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::malloc();
	}
	static void operator delete(void* _p)
	{
		boost::singleton_pool<T, sizeof(T), boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::free(_p);
	}
};

template<class T>
class PoolAllocator
{
public:
	static void* operator new(size_t _size)
	{
		return boost::pool_allocator<T, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::allocate(_size);
	}
	static void operator delete(void* _p)
	{
		boost::pool_allocator<T, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::deallocate((T* const)_p, sizeof(T));
	}
};

template<class T>
class FastPool
{
public:
	static void* operator new(size_t _size)
	{
		return boost::fast_pool_allocator<T, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::allocate();
	}
	static void operator delete(void* _p)
	{
		boost::fast_pool_allocator<T, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::deallocate((T* const)_p, sizeof(T));
	}
};

class malloc_free
{
public:
	static void* operator new(size_t size)
	{
		return malloc(size);
	}
	static void operator delete(void *p)
	{
		return free(p);
	}

	int _data;
};


class Test// : public ObjectPoolAlloc<Test>
{
public:
	Test() { cout << "hello" << endl; }
	~Test() { cout << "world" << endl; }
};
//class Test : public Alloc<Test>
//{
//public:
//	Test() { cout << "hello" << endl; }
//	~Test() { cout << "world" << endl; }
//};


// 메모리 풀로 풀 안에 리소스 접근 가능성 X -> ResourceManager에서 Pool들을 관리 하려고 했는데 그럴필요가 없다.
// pool을 자료구조에 사용할 일도 없음.
//boost::pool_allocator<Test> TestPool;

int main()
{
	Timer::SetUp();
	Timer t;	
	t.start();

	/*boost::object_pool<Test> p;

	Test **temp = new Test*[100000];
	for (int i = 0; i < 100000; i++)
	{
		temp[i] = new Test();
	}*/

	//boost::singleton_pool<boost::pool_allocator_tag, sizeof(Test)>::release_memory();
	//boost::singleton_pool<Test, sizeof(Test), boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>::release_memory();

	/*for (int i = 0; i < 100000; i++)
	{
		p.destroy(temp[i]);
	}*/
	
	//delete[] temp;

	

	//boost::shared_ptr<Test> temp = Alloc<Test>::allocate();

	
	//boost::shared_ptr<Test> a(boost::fast_pool_allocator<Test>::allocate());

	//for (int i = 0; i < 1000000; i++) // 864
	//{
	//	Test * test = boost::fast_pool_allocator<Test>::allocate();
	//	boost::fast_pool_allocator<Test>::deallocate(test);
	//}

	//for (int i = 0; i < 1000000; i++) // 2702
	//{
	//	Test * test = boost::pool_allocator<Test>::allocate(sizeof(Test));
	//	boost::pool_allocator<Test>::deallocate(test, sizeof(Test));
	//}
	

	boost::object_pool<Test> p(10000);
	for (int i = 0; i < 10000; ++i)
	{
		//Test * const t = p.malloc();//생성자 호출 X
		//p.free(t) //소멸자 호출 X

		Test * const t = p.();//생성자 호출 ㅒ

		p.free(t); //소멸자 호출
	}

	/*for (int i = 0; i < 10000; i++)
	{
		Test * temp = new Test();	

		delete temp;
	}*/

		


	t.update();

	cout << t.getElapsedTime() << " Millisecond" << endl;
}