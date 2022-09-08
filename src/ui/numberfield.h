#pragma once

#include "editablelabel.h"

class NumberField : public EditableLabel {
    OBJ_DEFINITION(NumberField, EditableLabel);

   public:
    NumberField();
    NumberField(const Real& number);
    NumberField(const Variable& p_variable) : EditableLabel(p_variable) {}

    virtual ~NumberField();

    vec2 get_required_size() const override;

    void notification(int p_notification) override;

    void handle_input(char c) override;
    void set_text(const String& p_text) override;

    void value_changed() override;

    static void bind_methods();
};
