#pragma once
#include "stdafx.h"
#include "Singleton.h"

class XFile : public Singleton<XFile>
{
private:
	LPD3DXFILE _d3dxFile;


public:
	XFile();
	~XFile();
	
	bool Init() final;
	void Release() final;

	LPD3DXFILE GetXFile();
};

#define XFILE XFile::Instance()
#define D3DXFILE XFile::Instance()->GetXFile()
