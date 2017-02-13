#pragma once
#include "..\Core\Core.h"

class Device : public Singleton<Device>
{
	friend class Singleton<Device>;
public:	
	~Device();

	bool Init() final;
	void Release() final;

	IDirect3DDevice9* Get() const;

private:
	Device();

private:
	IDirect3D9*         _d3d;
	IDirect3DDevice9*   _d3dDevice;
};

inline IDirect3DDevice9* Device::Get() const
{
	return _d3dDevice;
}
