#include "../../Object/ObjectManager.h"
#include "Render.h"

Render::~Render()
{
}

void Render::Begin(ObjectManager& objectManager)
{
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
}


