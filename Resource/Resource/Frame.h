#pragma once
#include "IXFileData.h"
#include "stdafx.h"
#include "Structure.h"

class AnimationController;
class ResourceManager;

class Frame : public IXFileData
{	
	friend class AnimationController;
	friend class ResourceManager;
private:
	MATRIXCONTAINER* _matrixContainer;	

public:
	virtual void save() override;
	virtual void loadChildren(LPD3DXFILEDATA, void*);
	virtual void load(LPD3DXFILEDATA) override;

public:	
	Frame();
	~Frame();
};

