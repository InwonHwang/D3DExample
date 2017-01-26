#include "Transform.h"
#include "..\ResourceBase\TransformData.h"


Transform::Transform()	
{
	_components = new std::vector<sp<Component>>();
	TransformImpl* impl = Memory<TransformImpl>::OrderedAlloc(sizeof(TransformImpl));
	_impl.reset(impl, Memory<TransformImpl>::OrderedFree);
}

Transform::~Transform()
{
}

void Transform::Destroy()
{	
	Clear();
	SafeDelete<std::vector<sp<Component>>>(_components);
}

void Transform::Update()
{
	_impl->Update();
}

void Transform::SetLocalScale(const Vector3& scale)
{
	_impl->SetLocalScale(scale);
}

void Transform::SetLocalRotation(const Quaternion& rotation)
{
	_impl->SetLocalRotation(rotation);
}

void Transform::SetLocalPosition(const Vector3& position)
{
	_impl->SetLocalPosition(position);
}


void Transform::Clear()
{
	for (auto c : *_components)
	{
		c->Destroy();
	}
	_components->clear();
}