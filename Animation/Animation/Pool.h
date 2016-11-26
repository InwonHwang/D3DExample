#pragma once
#include "stdafx.h"

class Mesh;
class Material;
class Texture;
class Component;

//namespace Pool
//{
//	static boost::object_pool<Mesh> MeshPool(10);
//}

class ResourcePool abstract
{
public:
	
};

class ComponentPool abstract
{
public:
	//std::vector<boost::pool_allocator<Renderer>> Renderers;
	// Exiting the function does NOT free the system memory allocated by the pool allocator.
	// You must call
	//boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::release_memory();
	// in order to force freeing the system memory.
};

