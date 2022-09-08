#pragma once

#include "types/callable.h"
#include "types/method.h"

class Property : public Callable {
   public:
    Property();
    ~Property();

    StringName var_name = "";
    VariantType inherits_from;
    VariantType var_type;

    R_Method_1* get;
    V_Method_2* set;
};

struct Variable {
    Property* property;
    Variant variant;

    Variant get() {
        if (property->get)
            return property->get->operator()(variant);
        else
            return NULL_VAR;
    }

    void set(const Variant& p_new) {
        property->set->operator()(variant, p_new);
    }
};
