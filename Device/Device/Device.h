#pragma once
#include <d3d9.h>
#include "Singleton.h"

#define SAFE_RELEASE(p) if(p) { p->Release(); }
#define SAFE_DELETE(p) if(p) { delete p; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; }

class Device : public Singleton<Device>
{
private:
	LPDIRECT3D9         _d3d;
	LPDIRECT3DDEVICE9   _d3dDevice;
	

public:
	Device();
	~Device();

	bool Init() override;
	void Clear() override;

	LPDIRECT3DDEVICE9 D3DDevice()
	{
		return _d3dDevice;
	}
};

#define D3D Device::Instance()
#define D3DDevice Device::Instance()->D3DDevice()

