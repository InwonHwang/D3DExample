#pragma once
#include "stdafx.h"

class IResourceItem abstract
{
//protected:
public:
	virtual ~IResourceItem() {}
	virtual void Save() = 0;
	virtual void Load(LPD3DXFILEDATA xfileData) = 0;
	virtual void Load(TCHAR* fileName) = 0;
	//virtual IResourceItem* Clone() = 0;
	//template<typename T> T* Share() { return static_cast<T *>(this); }
};
