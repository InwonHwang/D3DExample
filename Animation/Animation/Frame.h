#pragma once
#include "IResourceItem.h"
#include "stdafx.h"

class Frame : public IResourceItem
{	
public:
	D3DXMATRIX* _matrix;
	

public:
	virtual void save() override;
	virtual void loadChild(LPD3DXFILEDATA, void*) = 0;
	virtual void load(LPD3DXFILEDATA) override;	
	virtual void load(TCHAR *) override;

public:	
	Frame();
	~Frame();
};

