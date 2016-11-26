#include "Frame.h"



Frame::Frame()
{
	_matrix = new D3DXMATRIX();
}


Frame::~Frame()
{
	SAFE_DELETE(_matrix);
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
		*_matrix = *tempMatrix;
		xfileData->Unlock();
	}
}

void Frame::load(TCHAR * fileName)
{

}
