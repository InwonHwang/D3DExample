#pragma once
#include "stdafx.h"

class Mesh;
class Material;
class Texture;

//namespace ResourcePool
//{
//	static boost::object_pool<Mesh> MeshPool(10);
//}

class ResourcePool abstract
{
public:
	static boost::object_pool<Mesh> MeshPool;
	static boost::object_pool<Material> MaterialPool;
	static boost::object_pool<Texture> TexturePool;
};

