#pragma once

#include "core/array.h"
#include "math/math.h"

struct PathElement {
    vec3 pos;
};

class Path {
   public:
   private:
    Array<PathElement> elements;
};
