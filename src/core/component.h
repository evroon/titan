#pragma once

#include "core/object.h"

class WorldObject;

class Component : public Object {
    OBJ_DEFINITION(Component, Object)

   public:
    Component();
    virtual ~Component();

    virtual void update() {}

    virtual void bind_parent(WorldObject* pa);

    static void bind_methods();

    WorldObject* parent;
};
