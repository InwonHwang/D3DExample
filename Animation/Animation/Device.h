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

	bool Init() final;
	void Release() final;

	LPDIRECT3DDEVICE9 D3DDevice()
	{
		return _d3dDevice;
	}
};

#define D3D Device::Instance()
#define D3DDevice D3D->D3DDevice()

