#pragma once
#include "stdafx.h"
#include <rmxfguid.h>

class IXFileData abstract
{
public:
	virtual ~IXFileData() {}
	virtual void save() = 0;
	virtual void load(ID3DXFileData* xfileData) = 0;	

	void getGUID(ID3DXFileData* xfileData, GUID* guid)
	{
		HRESULT hr = 0;
		if (FAILED(hr = xfileData->GetType(guid)))
			DebugError(hr);		
	}

	void getName(ID3DXFileData* xfileData, char **name)
	{
		DWORD size = 0;
		HRESULT hr = 0;

		if (FAILED(hr = xfileData->GetName(NULL, &size)))
		{
			DebugError(hr);
			return;						
		}		

		if (size != 0) {
			*name = new char[size];
			if (FAILED(hr = xfileData->GetName(*name, &size)))
				DebugError(hr);
		}	
	}

	bool contains(ID3DXFileData* xfileData, GUID guid)
	{
		GUID type;
		HRESULT hr = 0;
		SIZE_T childCount = 0;		
		
		getGUID(xfileData, &type);

		if (guid == type) return true;

		xfileData->GetChildren(&childCount);		

		for (SIZE_T i = 0; i < childCount; i++)	{
			ID3DXFileData* data = NULL;

			xfileData->GetChild(i, &data);
			if (contains(data, guid))
			{
				SAFE_RELEASE(data);
				return true;
			}
			SAFE_RELEASE(data);
		}

		return false;
	}
};

class ISubDataParser
{
public:
	virtual ~ISubDataParser() {}
	virtual void loadChildren(ID3DXFileData* xfileData, void* pData) = 0;
	virtual void saveChildren(ID3DXFileData* xfileData, void* pData) = 0;
};
