#pragma once

#include "control.h"

class PropertyControl : public Control {
    OBJ_DEFINITION(PropertyControl, Control);

   public:
    PropertyControl();
    PropertyControl(const Variant &p_variant);
    PropertyControl(const Variable &p_variable);

    virtual ~PropertyControl();

    void update_value();
    virtual void value_changed() {}

    virtual void value_input() {}

    // use externally to bind a value or variable to the control
    void set_value(const Variant &p_value);
    void set_value(const Variable &p_variable);

    Variant get_value();
    VariantType get_property_type();

    static void bind_methods();

   protected:
    // use when the value has been set by user input
    // note: prevents an infinite loop
    void value_set(const Variant &p_value);

   private:
    union {
        Variant variant;
        Variable variable;
    };

    enum Type { VARIANT, VARIABLE } type;
};
