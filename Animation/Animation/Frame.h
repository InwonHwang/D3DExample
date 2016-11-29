#pragma once
#include "IResourceItem.h"
#include "stdafx.h"
#include "Structure.h"

class AnimationController;

class Frame : public IResourceItem
{	
	friend class AnimationController;
private:
	MATRIXCONTAINER* _matrixContainer;	

public:
	virtual void save() override;
	virtual void loadChild(LPD3DXFILEDATA, void*);
	virtual void load(LPD3DXFILEDATA) override;	
	virtual void load(TCHAR *) override;

public:	
	Frame();
	~Frame();
};

