#include "transformcomponent.h"

#include "graphics/rendercomponent.h"
#include "world/worldobject.h"

void TransformComponent::update() {
    RenderComponent* r =
        (RenderComponent*)((WorldObject*)parent)->rendercomponent;

    if (!r->isstatic && r->isvisible) transform.update();
}

void TransformComponent::set_transform(const Transform& p_transform) {
    transform = p_transform;
}

Transform TransformComponent::get_transform() const { return transform; }

#undef CLASSNAME
#define CLASSNAME TransformComponent

void TransformComponent::bind_methods() {
    REG_CSTR(0);

    REG_PROPERTY(transform);
}
