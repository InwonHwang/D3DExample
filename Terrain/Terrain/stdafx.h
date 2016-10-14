#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxfile.h>
#include <DxErr.h>
#include <tchar.h>
#include <string>
#include <vector>

#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; }

namespace std { typedef basic_string<TCHAR> tstring; }

void DebugError(HRESULT hr);
void DebugBox(HRESULT hr, LPCTSTR str);
float Lerp(float a, float b, float t);