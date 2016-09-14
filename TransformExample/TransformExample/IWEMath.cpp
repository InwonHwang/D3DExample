#include "IWEMath.h"

int IWEMath::Abs(int f)
{
	return abs(f);
}

float IWEMath::Abs(float f)
{
	return abs(f);
}

float IWEMath::Acos(float f)
{
	return acosf(f);
}

float IWEMath::Asin(float f)
{
	return asinf(f);
}
float IWEMath::Atan(float f)
{
	return atanf(f);
}
float IWEMath::Atan2(float y, float x)
{
	return atan2f(y, x);
}

float IWEMath::Sin(float f)
{
	return sinf(f);
}
float IWEMath::Cos(float f)
{
	return cosf(f);
}
float IWEMath::Tan(float f)
{
	return tanf(f);
}

float IWEMath::Sqrt(float f)
{
	return sqrtf(f);
}

float IWEMath::Log(float f)
{
	return std::logf(f);
}

float IWEMath::Log10(float f)
{
	return std::log10f(f);
}

float IWEMath::Round(float f)
{
	return roundf(f);
}

float IWEMath::Exp(float f)
{
	return expf(f);
}
float IWEMath::Floor(float f)
{
	return floorf(f);
}

float IWEMath::Ceil(float f)
{
	return ceilf(f);
}


float IWEMath::Clamp(float f, float min, float max)
{		
	return std::fmax(min, std::fmin(f, max));
}