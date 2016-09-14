#include "IWEGameObject.h"
#include "IWETransform.h"
#include "IWEMesh.h"

GameObject::GameObject()
	:
	mesh(NULL),
	transform(NULL)
{
	transform = new Transform();
}

GameObject::~GameObject()
{	
	SAFE_DELETE(mesh);	
	SAFE_DELETE(transform);
}

void GameObject::update()
{
	if (transform) transform->update();
	if (mesh) mesh->update();

}
