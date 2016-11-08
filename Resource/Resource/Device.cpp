#include "Device.h"

Device::Device()
	: Singleton(),
	_d3d(NULL),
	_d3dDevice(NULL)
{
}

Device::~Device()
{
}

bool Device::Init()
{
	if (NULL == (_d3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return FALSE;
		
	D3DDISPLAYMODE d3ddm;
	if (FAILED(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		SAFE_RELEASE(_d3d);
		return FALSE;
	}

	HRESULT hr;
	if (FAILED(hr = _d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE, D3DFMT_D16)))
	{
		if (hr == D3DERR_NOTAVAILABLE)
		{
			SAFE_RELEASE(_d3d);
			return false;
		}
	}
	D3DCAPS9 d3dCaps;
	if (FAILED(_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
	{
		SAFE_RELEASE(_d3d);
		return false;
	}
	

	DWORD vp = 0;
	if (d3dCaps.VertexProcessingCaps != 0)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//d3dpp.BackBufferWidth = 200;
	//d3dpp.BackBufferHeight = 200;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetActiveWindow();
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetActiveWindow(), vp, &d3dpp, &_d3dDevice)))
	{
		SAFE_RELEASE(_d3d);
		return false;
	}	

	return true;
}

void Device::Release()
{
	SAFE_RELEASE(_d3dDevice);
	SAFE_RELEASE(_d3d);
}
