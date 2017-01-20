#pragma once

#include "../ThirdParty/ThirdParty.h"

// Memory 클래스
// 자주 할당되고 해제되는 변수들에 대해서만 사용하길 권장.

class Memory abstract
{
	
public:
	static void* Alloc() {}
	static void Free(void * p) {}

	static void Release() {}
	static void Clear() {}	//할당되었던 모든 메모리 클리어
};

#define ALLOC() Memory::Alloc()
#define FREE(p) Memory::Free(p)
//#define SAFE_RELEASE() Memory::Release()
#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }