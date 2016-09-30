#pragma once
#include "stdafx.h"

class Color : public D3DXCOLOR
{
public:
	Color() {}
	Color(const Color& color) : D3DXCOLOR(color) {}
	Color(const D3DXCOLOR& color) : D3DXCOLOR(color) {}
	Color(float r, float g, float b, float a) : D3DXCOLOR(r, g, b, a) {}
	~Color() {}
};