#pragma once

#include "control.h"

class Space : public Control {
    OBJ_DEFINITION(Space, Control)

   public:
    Space(vec2 p_size) { size = p_size; }

    vec2 get_required_size() const override { return size; }

   private:
    vec2 size;
};
