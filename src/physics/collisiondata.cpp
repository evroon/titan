#include "collisiondata.h"

#include "core/titanscript/scriptcomponent.h"
#include "input/eventhandler.h"

void CollisionData::BeginContact(b2Contact* contact) {
    WorldObject* A =
        reinterpret_cast<WorldObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    WorldObject* B =
        reinterpret_cast<WorldObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    CollisionEvent* e = new CollisionEvent;
    e->object = A;
    e->contact = CollisionEvent::ContactType::BEGIN;

    B->handle_event(e);
}

void CollisionData::EndContact(b2Contact* contact) {
    WorldObject* A =
        reinterpret_cast<WorldObject*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    WorldObject* B =
        reinterpret_cast<WorldObject*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    CollisionEvent* e = new CollisionEvent;
    e->object = A;
    e->contact = CollisionEvent::ContactType::END;

    B->handle_event(e);
}
