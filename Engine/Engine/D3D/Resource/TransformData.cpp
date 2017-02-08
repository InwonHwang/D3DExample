#include "TransformData.h"



TransformData::TransformData(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
	D3DXMatrixIdentity(&_transform);
	D3DXMatrixIdentity(&_transformParent);
}


TransformData::~TransformData()
{
}

void TransformData::Destroy()	// ��ǻ� �ʿ����...
{
	D3DXMatrixIdentity(&_transform);
	D3DXMatrixIdentity(&_transformParent);
	ResourceItem::Unload();
	ResourceItem::Clear();
}