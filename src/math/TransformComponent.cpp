#include "TransformComponent.h"

#include "graphics/RenderComponent.h"
#include "world/WorldObject.h"

void TransformComponent::update()
{
	RenderComponent *r = (RenderComponent*)((WorldObject*)parent)->rendercomponent;

	if (!r->isstatic && r->isvisible)
		transform.update();
}