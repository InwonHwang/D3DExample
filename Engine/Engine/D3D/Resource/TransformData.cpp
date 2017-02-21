#include "TransformData.h"



TransformData::TransformData(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
	D3DXMatrixIdentity(&_matLocal);
	D3DXMatrixIdentity(&_matWorldParent);	
}


TransformData::~TransformData()
{	
}

void TransformData::Destroy()	// 사실상 필요없음...
{
	D3DXMatrixIdentity(&_matLocal);
	D3DXMatrixIdentity(&_matWorldParent);
	ResourceItem::Unload();
	ResourceItem::Clear();
}