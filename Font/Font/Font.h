#pragma once
#include "Color.h"

class Font
{

private:
	LPD3DXFONT _d3dxFont;
	std::tstring _string;
	RECT _rect;		//���Ŀ� RectTransform ���� �� ����� ����
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
	void setRect(float left, float top, float right, float bottom); //���Ŀ� RectTransform ���� �� ����� ����
	void draw(); // ���߿� component���� ��� ����
	void update(); // ���߿� component���� ��� ����
	void release(); // ���߿� component���� ��� ����
	
};

