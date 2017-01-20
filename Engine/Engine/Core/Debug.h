#pragma once
#include <Windows.h>
#include "DataTypes.h"

class Debug
{
public:
	Debug();
	~Debug();

	static void MsgBox(HRESULT hr, LPCTSTR str);
	static String IntegerToString(int i);
	static String FloatToString(float f);
};

