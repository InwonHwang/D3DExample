#pragma once
#include "..\Core\Core.h"

class Device : public Singleton<Device>
{
private:
	IDirect3D9*         _d3d;
	IDirect3DDevice9*   _d3dDevice;

public:
	Device();
	~Device();

	bool Init() final;
	void Release() final;

	IDirect3DDevice9* GetD3DDevice() const;
};

inline IDirect3DDevice9* Device::GetD3DDevice() const
{
	return _d3dDevice;
}
