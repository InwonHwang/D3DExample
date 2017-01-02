#pragma once

#include <assert.h>
#include <tchar.h>
#include <string>
#include <Windows.h>

#include "BitFlags.h"
#include "ResourceItem.h"
#include "Singleton.h"
#include "STL.h"

#define SAFE_DELETE(x) { if(x) { delete x; x = nullptr; } }
#define SAFE_DELETE_ARRAY(x){ if(x) { delete[] x; x = nullptr; } }
#define SAFE_RELEASE(x) { if(x) { x->Release(); } }

enum TYPE {
	DYNAMIC,
	RAMBACKUP,	// 힙 메모리에 저장.
	VOLITILE
};

namespace std { typedef basic_string<TCHAR> tstring; }

void DebugBox(HRESULT hr, LPCTSTR str);

std::tstring floatToString(float f);

float Lerp(float a, float b, float t);