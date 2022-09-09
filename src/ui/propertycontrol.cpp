#include "propertycontrol.h"

PropertyControl::PropertyControl() { set_value(Variant()); }

PropertyControl::PropertyControl(const Variant& p_value) { set_value(p_value); }

PropertyControl::PropertyControl(const Variable& p_variable) { set_value(p_variable); }

PropertyControl::~PropertyControl() {}

void PropertyControl::update_value() {
    // if (type == VARIABLE)
    //	set_value(variable.get());

    value_changed();
    update();
}

void PropertyControl::set_value(const Variant& p_value) {
    if (get_focused()) return;

    type = VARIANT;
    variant = p_value;

    value_changed();
    update();
}

void PropertyControl::set_value(const Variable& p_variable) {
    if (get_focused()) return;

    type = VARIABLE;
    variable = p_variable;

    value_changed();
    update();
}

Variant PropertyControl::get_value() {
    if (type == VARIANT) {
        if (variant.isdef()) return variant;

        return NULL_VAR;
    }

    return variable.get();
}

VariantType PropertyControl::get_property_type() {
    if (type == VARIABLE) return variable.property->var_type;

    return variant.get_type();
}

void PropertyControl::value_set(const Variant& p_value) {
    if (type == VARIANT) {
        variant = p_value;
    } else {
        variable.set(p_value);
    }
    update();
}

#undef CLASSNAME
#define CLASSNAME PropertyControl

void PropertyControl::bind_methods() {
    REG_CSTR_OVRLD_1(Variant);

    REG_METHOD(value_changed);

    REG_SIGNAL("value_changed");
}
