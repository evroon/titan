#include "world.h"

#include "core/corenames.h"
#include "graphics/renderer.h"
#include "graphics/view.h"
#include "physics/physicsworld.h"
#include "world/light.h"
#include "world/terrain.h"

World::World() {
    AddLayer(new Layer(0, "DefaultLayer"));

    physics_2d = new PhysicsWorld2D;
    physics_3d = NULL;
    active_camera = NULL;
}

World::~World() {}

void World::add_child(Node* p_child) {
    WorldObject* world_object = p_child->cast_to_type<WorldObject*>();

    if (!world_object) return;

    Node::add_child(world_object);
    world_object->register_in_world(this);
    world_object->set_layer(GetLayer(0));

    if (!active_camera && p_child->is_of_type<Camera>())
        set_active_camera(world_object->cast_to_type<Camera*>());

    world_object->notificate(WorldObject::NOTIFICATION_READY);
}

void World::remove_child(Node* p_child) { Node::remove_child(p_child); }

WorldObject* World::get_worldobject(const String& name) {
    for (Node* obj : children) {
        if (obj->get_name() == name) return obj->cast_to_type<WorldObject*>();
    }

    return nullptr;
}

Viewport* World::get_viewport() const { return ACTIVE_VIEWPORT; }

Layer* World::GetLayer(const String& name) {
    for (int c = 0; c < layers.size(); c++)
        if (layers[c]->name == name) return layers[c];

    return nullptr;
}

Layer* World::GetLayer(int depth) {
    for (int c = 0; c < layers.size(); c++)
        if (layers[c]->depth == depth) return layers[c];

    return nullptr;
}

void World::AddLayer(Layer* l) {
    int destination = -1;

    for (int c = 0; c < layers.size(); c++) {
        if (layers[c]->depth > l->depth) {
            destination = c;
            break;
        }
    }

    if (destination == -1)
        layers.push_back(l);
    else
        layers.insert(destination, l);
}
void World::RemoveLayer(Layer* l) { layers.clear(l); }

void World::RepositionLayer(Layer* l) {
    RemoveLayer(l);
    AddLayer(l);
}

void World::handle_event(Event* e) {
    for (Node* p : children) p->cast_to_type<WorldObject*>()->handle_event(e);
}

void World::init() {
    if (has_script()) run(CORE_NAMES->ready, Arguments());
}

void World::update() {
    if (has_script()) run(CORE_NAMES->update, Arguments());

    physics_2d->update();

    for (Layer* l : layers)
        for (WorldObject* wo : l->objects)
            wo->notificate(WorldObject::NOTIFICATION_UPDATE);
}

void World::draw() {
    for (Node* o : children) {
        WorldObject* wo = o->cast_to_type<WorldObject*>();
        if (wo->get_visible()) wo->notificate(WorldObject::NOTIFICATION_DRAW);
    }
}

void World::Free() { children.clean(); }

void World::set_active_camera(Camera* p_camera) { active_camera = p_camera; }

Camera* World::get_active_camera() const { return active_camera; }

DirectionalLight* World::get_active_light() const {
    return ACTIVE_WORLD->get_child_by_type<DirectionalLight*>();
}

PhysicsWorld2D* World::get_physics_2d() const { return physics_2d; }

PhysicsWorld3D* World::get_physics_3d() const { return physics_3d; }

#undef CLASSNAME
#define CLASSNAME World

void World::bind_methods() {
    REG_CSTR(0);

    REG_METHOD(get_viewport);
}
