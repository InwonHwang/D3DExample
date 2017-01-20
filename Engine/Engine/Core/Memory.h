#pragma once

#include "../ThirdParty/ThirdParty.h"

// Memory Ŭ����
// ���� �Ҵ�ǰ� �����Ǵ� �����鿡 ���ؼ��� ����ϱ� ����.

class Memory abstract
{
	
public:
	static void* Alloc() {}
	static void Free(void * p) {}

	static void Release() {}
	static void Clear() {}	//�Ҵ�Ǿ��� ��� �޸� Ŭ����
};

#define ALLOC() Memory::Alloc()
#define FREE(p) Memory::Free(p)
//#define SAFE_RELEASE() Memory::Release()
#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }