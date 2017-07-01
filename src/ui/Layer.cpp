#include "Layer.h"

#include "world/WorldObject.h"

void Layer::Bind(WorldObject *o)
{
	objects.push_back(o);
}

void Layer::UnBind(WorldObject *o)
{
	objects.clear(o);
}