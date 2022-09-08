#pragma once

#include "uibox.h"

class Seperator : public Control {
    OBJ_DEFINITION(Seperator, Control);

   public:
    Seperator();
    ~Seperator();

    vec2 get_required_size() const override;

    void notification(int p_notification) override;

   private:
    Color color;
};
