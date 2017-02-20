#pragma once
#include <string>
#include <tchar.h>
#include <typeinfo>	// RTTI 정보 활용... 속도 저하 추후에 직접 구현하는 방식으로 변경

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned int	uint;

typedef __int64   int64;
typedef unsigned __int64  uint64;

typedef std::basic_string<TCHAR> String;

class StringUtil
{
public:
	inline static void SetName(const char* str, String& dest)
	{
		std::string temp = str;
		dest.assign(temp.begin(), temp.end());
	}

	inline static String FloatToString(float f)
	{
		std::string temp = std::to_string(f);
		String ret;
		ret.assign(temp.begin(), temp.end());

		return ret;
	}

	inline static String IntegerToString(int i)
	{
		std::string temp = std::to_string(i);
		String ret;
		ret.assign(temp.begin(), temp.end());

		return ret;
	}
};


//class String : public TString
//{
//public:
//	LPCSTR string
//};
