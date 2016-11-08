#pragma once
#include "stdafx.h"

class IResourceItem abstract
{
public:
	enum RESOURCE_TYPE {
		RESOURCE_SHAREABLE,
		RESOURCE_CLONABLE
	};
protected:
	bool _isShareable;
	bool _isClonable;

protected:
	virtual ~IResourceItem() {}
	virtual void Save() = 0;
	virtual void Load(LPD3DXFILEDATA xfileData) = 0;
	virtual void Load(TCHAR* fileName) = 0;
	//virtual IResourceItem* Clone() = 0;
	template<typename T> T* Share() { return static_cast<T *>(this); }
};
