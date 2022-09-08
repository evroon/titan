#pragma once

#include "core/string.h"

#define CORE_NAMES CoreNames::get_singleton()
#define CORE_TYPE(X) CORE_NAMES->X

class CoreNames {
   public:
    CoreNames();
    ~CoreNames();

    static void init();

    // CoreTypes
    const StringName Null = "NULL";
    const StringName Bool = "bool";
    const StringName Int = "int";
    const StringName Float = "float";
    const StringName String = "String";
    const StringName vec2 = "vec2";
    const StringName vec3 = "vec3";
    const StringName vec4 = "vec4";
    const StringName mat4 = "Mat4";
    const StringName Color = "Color";
    const StringName Transform = "Transform";
    const StringName Object = "Object";
    const StringName Array = "Array";

    const StringName Real = "Real";

    const StringName ready = "ready";
    const StringName draw = "draw";
    const StringName update = "update";
    const StringName handle_event = "handle_event";

    // static
    static CoreNames *get_singleton();

    static CoreNames *singleton;
};
