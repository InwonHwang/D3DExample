#pragma once
#include <Windows.h>
#include <DxErr.h>
#include "DataTypes.h"

class Debug
{
public:
	Debug();
	~Debug();

	static void MsgBox(HRESULT hr, LPCTSTR str);
	static void D3DError(HRESULT hr);

	//���Ŀ� �ٸ������� �ű��
	static String IntegerToString(int i);
	static String FloatToString(float f);
};

#define DebugBox(hr, str) Debug::MsgBox(hr, str)
#define DebugError(hr) Debug::D3DError(hr)