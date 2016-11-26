#include "XFile.h"
#include "iwrmxftmpl.h"

XFile::XFile()
{
}


XFile::~XFile()
{
}

bool XFile::Init()
{
	HRESULT hr;

	if (FAILED(hr = D3DXFileCreate(&_d3dxFile)))
	{
		DebugBox(hr, _T("D3DXFileCreate"));
		return FALSE;
	}
		
	if (FAILED(hr = _d3dxFile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		DebugBox(hr, _T("RegisterTemplates"));
		SAFE_RELEASE(_d3dxFile);
		return FALSE;
	}

	return TRUE;
}
void XFile::Release()
{
	SAFE_RELEASE(_d3dxFile);
}

LPD3DXFILE XFile::GetXFile()
{
	return _d3dxFile;
}
