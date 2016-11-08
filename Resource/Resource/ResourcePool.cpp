#include "ResourcePool.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

boost::object_pool<Mesh> ResourcePool::MeshPool;
boost::object_pool<Material> ResourcePool::MaterialPool;
boost::object_pool<Texture> ResourcePool::TexturePool;