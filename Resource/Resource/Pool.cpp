#include "Pool.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

boost::object_pool<Mesh> ResourcePool::Mesh;
boost::object_pool<Material> ResourcePool::Material;
boost::object_pool<Texture> ResourcePool::Texture;