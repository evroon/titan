#include "TransformComponent.h"

#include "RenderComponent.h"
#include "WorldObject.h"

void TransformComponent::update()
{
	RenderComponent *r = (RenderComponent*)((WorldObject*)parent)->rendercomponent;

	if (!r->isstatic && r->isvisible)
		transform.update();
}