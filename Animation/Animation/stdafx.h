#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxfile.h>
#include <DxErr.h>
#include <tchar.h>
#include <string>
#include <list>
#include <vector>

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/singleton_pool.hpp>

namespace std { typedef basic_string<TCHAR> tstring; }

void DebugError(HRESULT hr);
void DebugBox(HRESULT hr, LPCTSTR str);
std::tstring IntegerToString(DWORD i);
std::tstring floatToString(float f);

#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; }
#define STRING std::tstring
