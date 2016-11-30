#include "Frame.h"



Frame::Frame()
	: IResourceItem(),
	_matrixContainer(NULL)
{
	_matrixContainer = new MATRIXCONTAINER();
}


Frame::~Frame()
{
	SAFE_DELETE(_matrixContainer);
}

void Frame::save()
{

}

void Frame::load(LPD3DXFILEDATA xfileData)
{
	D3DXMATRIX* tempMatrix;
	SIZE_T Size;
	
	HRESULT hr = 0;
	if (SUCCEEDED(hr = xfileData->Lock(&Size, (LPCVOID*)&tempMatrix)))
	{
		_matrixContainer->Matrix = *tempMatrix;
		xfileData->Unlock();
	}
}

void Frame::loadChild(LPD3DXFILEDATA, void*)
{

}

void Frame::load(TCHAR * fileName)
{

}
