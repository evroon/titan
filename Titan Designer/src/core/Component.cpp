#include "Component.h"

#include "world/WorldObject.h"

Component::Component() { }


Component::~Component()
{
}

void Component::bind_parent(WorldObject *pa)
{
	parent = pa;
}

#undef CLASSNAME
#define CLASSNAME Component

void Component::bind_methods()
{
	REG_METHOD(bind_parent);
}
