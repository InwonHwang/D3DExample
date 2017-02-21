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

