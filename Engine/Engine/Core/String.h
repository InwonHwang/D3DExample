#pragma once
#include <tchar.h>
#include <string>

namespace std { typedef basic_string<TCHAR> tstring; }

typedef std::tstring String;

#define OUTPUT_TEXT(x)	x.c_str()
#define TEXT(x)			_T(x)
