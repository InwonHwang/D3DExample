#pragma once
#include "stdafx.h"

class IResourceItem abstract
{
public:
	virtual ~IResourceItem() {}

public:
	//virtual void save() = 0;
	//virtual void loadChild(LPD3DXFILEDATA xfileData, void* ppData) = 0;
	//virtual void load(LPD3DXFILEDATA xfileData) = 0;
	//virtual IResourceItem* Clone() = 0;
	//template<typename T> T* Share() { return static_cast<T *>(this); }
};
