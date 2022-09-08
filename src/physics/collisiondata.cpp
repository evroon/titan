#include "collisiondata.h"

#include "core/titanscript/scriptcomponent.h"
#include "input/eventhandler.h"

void CollisionData::BeginContact(b2Contact *contact) {
    WorldObject *A = static_cast<WorldObject *>(
        contact->GetFixtureA()->GetBody()->GetUserData());
    WorldObject *B = static_cast<WorldObject *>(
        contact->GetFixtureB()->GetBody()->GetUserData());

    CollisionEvent *e = new CollisionEvent;
    e->object = A;
    e->contact = CollisionEvent::ContactType::BEGIN;

    B->handle_event(e);
}

void CollisionData::EndContact(b2Contact *contact) {
    WorldObject *A = static_cast<WorldObject *>(
        contact->GetFixtureA()->GetBody()->GetUserData());
    WorldObject *B = static_cast<WorldObject *>(
        contact->GetFixtureB()->GetBody()->GetUserData());

    CollisionEvent *e = new CollisionEvent;
    e->object = A;
    e->contact = CollisionEvent::ContactType::END;

    B->handle_event(e);
}
