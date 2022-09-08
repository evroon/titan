#pragma once

#include "control.h"

class Slider : public Control {
    OBJ_DEFINITION(Slider, Control);

   public:
    Slider();

    void step_down();
    void step_up();

    float get_value() const;

    void set_slider_pos(float p_position);
    float get_slider_pos(const vec2 &pos) const;

    void handle_event(UIEvent *ui_event) override;
    void notification(int p_notification) override;
    vec2 get_required_size() const override;

    static void bind_methods();

   private:
    rect2 inner_box;

    float inner_height;
    float position;
    float space;

    vec2 grab_pos;
    bool is_dragging;
};
