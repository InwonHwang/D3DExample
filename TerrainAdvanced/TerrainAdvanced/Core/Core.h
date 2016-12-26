#pragma once

#include <assert.h>
#include <tchar.h>

#include "BitFlags.h"
#include "ResourceItem.h"
#include "Singleton.h"

#define SAFE_DELETE(x) { if(x) { delete x; x = nullptr; } }
#define SAFE_DELETE_ARRAY(x){ if(x) { delete[] x; x = nullptr; } }
#define SAFE_RELEASE(x) { if(x) { x->Release(); } }

enum TYPE {
	DYNAMIC,
	RAMBACKUP,	// 힙 메모리에 저장.
	VOLITILE
};