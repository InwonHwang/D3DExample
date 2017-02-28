#pragma once

#include "..\ThirdParty\ThirdParty.h"
#include "DataStructure.h"
#include "DataTypes.h"

typedef unsigned int Handle;

class TableImpl
{
public:
	typedef std::vector<Handle> HandleVec;

	enum
	{
		InvalidHandle = 0xffffffff
	};

public:
	TableImpl() :
		_IDCount(0)
	{
		_pUsableIDVec.reset(new HandleVec);
	}
	virtual ~TableImpl()
	{
		_pUsableIDVec.reset();
	}

protected:
	Handle GenerateResourceHandle(); // 새로 생생되는 리소스의 핸들을 생성.
	void AddID(Handle handle);
protected:
	sp<HandleVec> _pUsableIDVec;
	uint		  _IDCount;
};


