#pragma once
#include <string>
#include <tchar.h>

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned int	uint;

typedef __int64   int64;
typedef unsigned __int64  uint64;

typedef std::basic_string<TCHAR> String;

//class String : public std::basic_string<TCHAR>
//{
//public:
//	String(const TCHAR* p)
//		: std::basic_string<TCHAR>(p)
//	{ }
//	~String() {}
//
//	
//};