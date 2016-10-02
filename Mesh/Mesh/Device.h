#pragma once

#include "Singleton.h"
#include "stdafx.h"

class Device : public Singleton<Device>
{
private:
	LPDIRECT3D9         _d3d;
	LPDIRECT3DDEVICE9   _d3dDevice;	

public:
	Device();
	~Device();

	bool Init() override;
	void Release() override;

	LPDIRECT3DDEVICE9 D3DDevice()
	{
		return _d3dDevice;
	}
};

#define D3D Device::Instance()
#define D3DDevice Device::Instance()->D3DDevice()

