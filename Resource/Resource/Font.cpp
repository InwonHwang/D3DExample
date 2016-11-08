#include "Font.h"
#include "Device.h"


Font::Font()
	: _string(_T("hello world")),
	_color(Color(1.f, 1.f, 1.f, 1.f))	
{
	SetRect(&_rect, 0, 0, 0, 0);
	D3DXCreateFont(D3DDevice, -20, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		_T("2002L"), &_d3dxFont);
}
Font::Font(INT height,
	UINT width,
	UINT weight,
	UINT mipLevels,
	BOOL italic,
	DWORD charset,
	DWORD OutputRecision,
	DWORD Quality,
	DWORD PitchAndFamily,
	LPCTSTR facename)
{
	init(height, width, weight, mipLevels, italic, charset, OutputRecision, Quality, PitchAndFamily, facename);
}

Font::~Font()
{
}

void Font::init(INT height,
	UINT width,
	UINT weight,
	UINT mipLevels,
	BOOL italic,
	DWORD charset,
	DWORD OutputRecision,
	DWORD Quality,
	DWORD PitchAndFamily,
	LPCTSTR facename)
{
	D3DXCreateFont(D3DDevice, height, width, weight, mipLevels, italic, charset,
		OutputRecision, Quality, PitchAndFamily,
		facename, &_d3dxFont);
}

void Font::setColor(float r, float g, float b, float a)
{
	_color.r = r;
	_color.g = g;
	_color.b = b;
	_color.a = a;
}
void Font::setString(const TCHAR *string)
{
	_string = string;
}
void Font::setRect(float left, float top, float right, float bottom)
{
	_rect.left = left;
	_rect.top = top;
	_rect.right = right;	
	_rect.bottom = bottom;
}

void Font::draw()
{
	_d3dxFont->DrawText(NULL, _string.c_str(), -1, &_rect, DT_NOCLIP, _color);
}

void Font::update()
{

}

void Font::release()
{
	SAFE_RELEASE(_d3dxFont);
}