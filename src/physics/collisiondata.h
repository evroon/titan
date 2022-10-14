#pragma once

#include "box2d/box2d.h"
#include "rigidbody.h"
#include "world/worldobject.h"

class CollisionData : public b2ContactListener {
   public:
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};
