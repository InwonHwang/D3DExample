#pragma once

#include <tchar.h>
#include <fbxsdk.h>
#include <cassert>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include "BitFlags.h"

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }

namespace std { typedef basic_string<TCHAR> tstring; }

void DebugBox(HRESULT hr, LPCTSTR str);
void DebugError(HRESULT hr);

std::tstring floatToString(float f);
