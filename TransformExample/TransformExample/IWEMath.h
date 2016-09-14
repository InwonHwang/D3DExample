#pragma once

#include "stdafx.h"

using namespace std;

class IWEMath
{	
private:
	IWEMath() {}	

public:
	static float Infinity(){ return INFINITY; }
	static float RadToDeg() { return (float)(180 / M_PI); }
	static float DegToRad() { return (float)(M_PI / 180); }

	static int Abs(int);
	static float Abs(float);

	static float Acos(float);
	static float Asin(float);
	static float Atan(float);
	static float Atan2(float, float);

	static float Sin(float);
	static float Cos(float);
	static float Tan(float);

	static float Sqrt(float);
	static float Log(float);
	static float Log10(float);

	static float Round(float);
	static float Exp(float);
	static float Floor(float);

	static float Ceil(float);
	static float Clamp(float, float, float);
};

