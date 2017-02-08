#include "Transform.h"
#include "..\Resource\D3DResource.h"


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
	for (auto c : *_components)	
		c->Destroy();
	
	_components->clear();
	SafeDelete<std::vector<sp<Component>>>(_components);
}

void Transform::Update(IDirect3DDevice9& device)
{
	_impl->Update(device);
}

void Transform::UpdateWorldMatrix()
{
	_impl->UpdateWorldMatrix();
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