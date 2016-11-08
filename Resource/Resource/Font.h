#pragma once
#include "Color.h"

class Font
{

private:
	LPD3DXFONT _d3dxFont;
	std::tstring _string;
	RECT _rect;		//추후에 RectTransform 구현 후 사라질 예정
	Color _color;

	void init(INT height,
		UINT width,
		UINT weight,
		UINT mipLevels,
		BOOL italic,
		DWORD charset,
		DWORD OutputRecision,
		DWORD Quality,
		DWORD PitchAndFamily,
		LPCTSTR facename);

public:
	Font();
	Font(INT height,
		UINT width,
		UINT weight,
		UINT mipLevels,
		BOOL italic,
		DWORD charset,
		DWORD OutputRecision,
		DWORD Quality,
		DWORD PitchAndFamily,
		LPCTSTR facename);
	
	~Font();
	
	void setColor(float r, float g, float b, float a);
	void setString(const TCHAR *string);
	void setRect(float left, float top, float right, float bottom); //추후에 RectTransform 구현 후 사라질 예정
	void draw(); // 나중에 component에서 상속 에정
	void update(); // 나중에 component에서 상속 에정
	void release(); // 나중에 component에서 상속 에정
	
};

