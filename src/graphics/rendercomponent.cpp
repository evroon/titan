#include "rendercomponent.h"

void RenderComponent::set_color(const Color& p_color) { color = p_color; }

Color RenderComponent::get_color() const { return color; }

#undef CLASSNAME
#define CLASSNAME RenderComponent

void RenderComponent::bind_methods() {
    REG_CSTR(0);

    REG_PROPERTY(color);
}
