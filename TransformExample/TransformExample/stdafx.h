#pragma once

#define _USE_MATH_DEFINES

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <tchar.h>
#include <string>
#include <list>
#include <stdio.h>
#include <cmath>
#include "IWEMath.h"
#include "IWEVector3.h"
#include "IWEQuaternion.h"





typedef std::basic_string<TCHAR> tstring;

#define RELEASE_COM(p) if(p) { p->Release(); p = NULL; }
#define SAFE_RELEASE(p) if(p) p->Release();
#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = NULL; }

void DebugBox(HRESULT, LPCSTR);
void DebugBox(HRESULT, LPCWSTR);
void DebugError(HRESULT hr);
void Debug(float, LPCSTR);
