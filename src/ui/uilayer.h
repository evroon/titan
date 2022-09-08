#pragma once

#include "control.h"
#include "core/vector.h"

class UILayer {
   public:
    UILayer();
    ~UILayer();

    Vector<Control> controls;
};
